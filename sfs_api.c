
#include "sfs_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fuse.h>
#include <strings.h>
#include <signal.h>
#include "disk_emu.h"
#include "typedefs/inode_table.h"
#include "typedefs/directory.h"
#include "typedefs/file_descriptor_table.h"
#include "typedefs/super_block.h"
#include "utils/disk_utils.h"
#include "typedefs/inode.h"
#include "typedefs/free_bitmap.h"
#include "typedefs/file_descriptor.h"
#include "typedefs/indirect_block.h"
#include "typedefs/directory_entry.h"

#define LASTNAME_FIRSTNAME_DISK "adibpour_nima_sfs_disk.disk"

inode_table_t *inode_table = NULL;
directory_t *root_dir = NULL;
file_descriptor_table_t *fd_table = NULL;

void FREE_AND_EXIT();

void mksfs(int fresh) {
    if (!inode_table) inode_table = malloc(sizeof(inode_table_t));
    if (!root_dir) root_dir = malloc(sizeof(directory_t));
    if (!fd_table) fd_table = malloc(sizeof(file_descriptor_table_t));
    /* Set up the sig handlers */
    signal(SIGINT, (__sighandler_t) FREE_AND_EXIT);
    signal(SIGSTOP, (__sighandler_t) FREE_AND_EXIT);
    signal(SIGTSTP, (__sighandler_t) FREE_AND_EXIT);
    /* Clear all in-memory data structures */
    memset(inode_table, 0, sizeof(inode_table_t));
    memset(root_dir, 0, sizeof(directory_t));
    memset(fd_table, 0, sizeof(file_descriptor_table_t));
    /* Initialize all in-memory data structures */
    inode_table_init(inode_table);
    directory_init(root_dir);
    fd_table_init(fd_table);
    switch (fresh) {
        case 1: {
            remove(LASTNAME_FIRSTNAME_DISK);
            init_fresh_disk(LASTNAME_FIRSTNAME_DISK, BLOCK_SIZE, BLOCK_COUNT);

            /* Setup the super block */
            super_block_t super_block;
            super_block_init(&super_block);
            super_block.magic = 0xACBD0005; /* Arbitrary */
            super_block.block_size = BLOCK_SIZE;
            super_block.fs_size = BLOCK_COUNT;
            super_block.inode_table_length = (int) byteToBlock(sizeof(inode_table_t));
            super_block.root_dir_inode_index = 0;


            inode_table_setUsed(inode_table, 0); /* for root_dir */
            inode_t *root_dir_inode = &inode_table->nodes[0];
            root_dir_inode->size = sizeof(directory_t);
            root_dir_inode->data_pointrs[0] = ROOT_DIR_DATABLOCK_INDEX;
            root_dir_inode->data_pointrs[1] = ROOT_DIR_DATABLOCK_INDEX + 1;
            root_dir_inode->data_pointrs[2] = ROOT_DIR_DATABLOCK_INDEX + 2;

            free_bitmap_t free_bitmap;
            free_bitmap_init(&free_bitmap);

            directory_init(root_dir);

            /* Write everything to the disk */
            write_blocks(SUPERBLOCK_DATABLOCK_INDEX, 1, &super_block);
            write_blocks(INODETABLE_DATABLOCK_INDEX, 8, inode_table);
            write_blocks(FREE_BITMAP_DATABLOCK_INDEX, 1, &free_bitmap);
            write_blocks(ROOT_DIR_DATABLOCK_INDEX, 3, root_dir);

            break;
        }
        case 0: {
            init_disk(LASTNAME_FIRSTNAME_DISK, BLOCK_SIZE, BLOCK_COUNT);
            super_block_t super_block;
            read_from_disk(SUPERBLOCK_DATABLOCK_INDEX, BLOCK_SIZE, &super_block);
            read_from_disk(INODETABLE_DATABLOCK_INDEX, sizeof(inode_table_t), inode_table);
            read_from_disk(ROOT_DIR_DATABLOCK_INDEX, sizeof(directory_t), root_dir);
            break;
        }
        default:
            break;
    }
}

int sfs_getnextfilename(char *fname) {
    /* Crawl all entries of the root dir */
    for (int i = root_dir->currentIndex; i < INODE_COUNT - 1; ++i) {
        if (directory_isUsed(root_dir, i)) { /* Ignore the free entries */
            int lengthToRead = (int)
                    ((strlen(root_dir->entries[i].name) > MAX_FILE_NAME_LENGTH) ?
                     MAX_FILE_NAME_LENGTH :
                     strlen(root_dir->entries[i].name));
            memcpy(fname, root_dir->entries[i].name, (size_t) lengthToRead); /* Copy the name of the file onto parameter */
            root_dir->currentIndex++; /* Increment the current index to know where we were on the next try */
            return 1;
        }
    }
    root_dir->currentIndex = 0;
    return 0;
}

int sfs_getfilesize(const char *path) {
    int inodeIndex = directory_getInodeIndex(root_dir, path);
    if (inodeIndex < 0) return -1;
    return inode_table->nodes[inodeIndex].size;
}

int sfs_fopen(char *name) {
    if (strlen(name) > 20) {
        return -1;
    }
    int inode_index = directory_getInodeIndex(root_dir, name);
    int fd_index;
    if (inode_index > 0) { /* File was already created */
        fd_index = fd_table_contains_inode(fd_table, inode_index);
        if (fd_index >= 0) return fd_index; /* File was already open */
        fd_index = fd_table_nextFree(fd_table);
        if (fd_index < 0) return -1; /* No space left to open file */
        fd_table_setUsed(fd_table, fd_index);
        fd_table->table[fd_index].inode_index = inode_index;
        fd_table->table[fd_index].inode = &inode_table->nodes[inode_index];
        fd_table->table[fd_index].rw_pointer = inode_table->nodes[inode_index].size; /* Open in append mode */
    } else { /* File doesn't exist, create it */
        int dir_index = directory_nextFree(root_dir);
        if (dir_index < 0) return -1; /* No space in directory left to create file */
        inode_index = inode_table_nextFree(inode_table);
        if (inode_index < 0) return -1; /* No space in inode table left to create file */
        fd_index = fd_table_nextFree(fd_table);
        if (fd_index < 0) return -1; /* No space in fd table to open the file */

        directory_setUsed(root_dir, dir_index);
        inode_table_setUsed(inode_table, inode_index);
        fd_table_setUsed(fd_table, fd_index);
        /* Connect that entry to it's inode */
        directory_entry_setName(&root_dir->entries[dir_index], name);
        directory_entry_setInodeIndex(&root_dir->entries[dir_index], inode_index);

        inode_table->nodes[inode_index].size = 0;

        fd_table->table[fd_index].inode_index = inode_index;
        fd_table->table[fd_index].inode = &inode_table->nodes[inode_index];
        fd_table->table[fd_index].rw_pointer = inode_table->nodes[inode_index].size;

        write_blocks(INODETABLE_DATABLOCK_INDEX, (int) byteToBlock(sizeof(inode_table_t)), inode_table);
        write_blocks(ROOT_DIR_DATABLOCK_INDEX, (int) byteToBlock(sizeof(directory_t)), root_dir);
    }
    return fd_index;
}

int sfs_fclose(int fileID) {
    if (fileID < 0) return -1; /* Invalid parameters! */
    if (fd_table_isFree(fd_table, fileID)) return -1; /* Already closed file before */
    fd_table_setFree(fd_table, fileID);
    fd_init(&fd_table->table[fileID]);
    return 0;
}

int sfs_fread(int fileID, char *buf, int length) {
    if (fileID < 0 || length < 0) return -1; /* That's an invalid input! */
    if (fd_table_isFree(fd_table, fileID)) return -1; /* File isn't open, can't read! */
    file_descriptor_t *fd = &fd_table->table[fileID];
    inode_t *inode = &inode_table->nodes[fd->inode_index];
    indirect_block_t indirect_block;
    if (inode->indirectPointer > 0) {
        /* Only read the indirect block if it actually exists, elsewise we won't really use it */
        read_from_disk(FILES_DATABLOCK_INDEX + inode->indirectPointer, BLOCK_SIZE, &indirect_block);
    };
    /* If the current place of the file + what we want to read is bigger than the file, then
     * that's illegal and we'll only read the remaining part of the file, if not, we can read
     * all the length that the client asked for */
    int bytesLeft = (fd->rw_pointer + length > inode->size) ?
                    inode->size - fd->rw_pointer :
                    length;
    int bytesRead = 0;
    char *diskBuffer = malloc(BLOCK_SIZE);
    while (bytesLeft > 0) {
        /* Clear the temporary buffer */
        memset(diskBuffer, 0, BLOCK_SIZE);
        /* Which data block pointer in the inode to access */
        int inodeDataPtrIndex = fd->rw_pointer / BLOCK_SIZE;
        /* Where in that data block are we going to be */
        int inodeDataBlockStartIndex = fd->rw_pointer % BLOCK_SIZE;
        int spaceLeftInDataBlock = BLOCK_SIZE - inodeDataBlockStartIndex;
        /* If we can read all we need in that one data block, then do that, otherwise,
         * read whatever is left in the block and we'll deal with the rest on the next iteration(s) */
        int bytesToRead = bytesLeft > spaceLeftInDataBlock ? spaceLeftInDataBlock : bytesLeft;
        int dataBlockIndex;
        if (inodeDataPtrIndex < INODE_DATA_POINTER_COUNT) {
            /* Which block on the disk does the inode point to? */
            dataBlockIndex = inode->data_pointrs[inodeDataPtrIndex];
            /* If I did my math correct, this should not happen, but if it does,
             * we can't continue reading! */
            if (dataBlockIndex < 0) break;
        } else {
            /* We need to access the indirect pointer in this case and grab our data from the block
             * it points to on our current index */
            int indirectBlockIndex = indirect_block_nextFree(&indirect_block);
            if (indirectBlockIndex < 0) {
                return -1;
            } /* No more space left on the indirect block! */
            dataBlockIndex = indirect_block.pointers[inodeDataPtrIndex - INODE_DATA_POINTER_COUNT];
            if (dataBlockIndex < 0) break;
        }
        /* At this point we're either pointing to a data block from a direct pointer, or an indirect pointer,
         * whichever the case may be, we have the index of the datablock we want, so read it into the buffer */
        read_from_disk(FILES_DATABLOCK_INDEX + dataBlockIndex, BLOCK_SIZE, diskBuffer);
        /* We now copy the temporary buffer into the buffer that the client wants. However, we don't just dump the
         * whole thing there, we need to copy only the portion that is remaining at the end of the buffer considering
         * where in the block the rw pointer resides */
        memcpy(buf, diskBuffer + inodeDataBlockStartIndex, (size_t) bytesToRead);
        /* Progress the rw pointer */
        fd->rw_pointer += bytesToRead;
        /* Keep track of how far we are in the operation */
        bytesRead += bytesToRead;
        bytesLeft -= bytesToRead;
        /* Increment the buffer to continue in the next iteration(s) where we left off here */
        buf += bytesToRead;
    }
    /* No longer need the temporary disk buffer */
    free(diskBuffer);
    return bytesRead;
}

int sfs_fwrite(int fileID, const char *buf, int length) {
    if (fileID < 0 || length < 0) return -1; /* That's an invalid input! */
    int bytesWritten = 0;
    int bytesLeft = length;
    file_descriptor_t *fd = &fd_table->table[fileID];
    inode_t *inode = &inode_table->nodes[fd->inode_index];
    if (fd->rw_pointer + length > 268 * BLOCK_SIZE) return -1; /* Max file size to write is 256 * BLOCK SIZE */
    indirect_block_t indirect_block;
    indirect_block_init(&indirect_block);
    free_bitmap_t free_bitmap;
    read_from_disk(FREE_BITMAP_DATABLOCK_INDEX, sizeof(free_bitmap_t), &free_bitmap);
    char *diskBuffer = malloc(BLOCK_SIZE);
    while (bytesLeft > 0) {
        /* Clear the temporary buffer */
        memset(diskBuffer, 0, BLOCK_SIZE);
        /* Which data block pointer in the inode to access */
        int inodeDataPtrIndex = fd->rw_pointer / BLOCK_SIZE;
        /* Where in that data block are we going to be */
        int inodeDataBlockStartIndex = fd->rw_pointer % BLOCK_SIZE;
        int spaceLeftInDataBlock = BLOCK_SIZE - inodeDataBlockStartIndex;
        /* If we can fit all we need to write in that data block, be it, if not, just fill the block */
        int bytesToWrite = bytesLeft > spaceLeftInDataBlock ? spaceLeftInDataBlock : bytesLeft;
        int dataBlockIndex;
        if (inodeDataPtrIndex < INODE_DATA_POINTER_COUNT) {
            /* We can access the data blocks through the direct pointers, no indirect block needed yet */
            dataBlockIndex = inode->data_pointrs[inodeDataPtrIndex];
            if (dataBlockIndex < 0) {
                /* data block was not setup for this pointer, set it up*/
                dataBlockIndex = free_bitmap_nextFree(&free_bitmap);
                if (dataBlockIndex < 0) return -1; /* No data blocks available on disk! */
                free_bitmap_setUsed(&free_bitmap, dataBlockIndex);
                inode->data_pointrs[inodeDataPtrIndex] = dataBlockIndex;
            } else {
                /* Data block already exists, we need to read it first then write on it */
                read_from_disk(FILES_DATABLOCK_INDEX + dataBlockIndex, BLOCK_SIZE, diskBuffer);
            }
        } else {
            /* index is beyond number of data pointers in inode, access indirect block */
            if (inode->indirectPointer < 0) {
                /* First time accessing indirect pointer for inode, set it up */
                int indirectBlockIndex = free_bitmap_nextFree(&free_bitmap);
                if (indirectBlockIndex < 0) return -1; /* No blocks available */
                free_bitmap_setUsed(&free_bitmap, indirectBlockIndex);
                /* Assign to inode */
                inode->indirectPointer = indirectBlockIndex;
            } else {
                /* Indirect block was setup before, read the indirect block */
                read_from_disk(FILES_DATABLOCK_INDEX + inode->indirectPointer, BLOCK_SIZE, &indirect_block);
            }
            /* We have the indirect block now, does it point to a valid block at our desired index? */
            dataBlockIndex = indirect_block.pointers[inodeDataPtrIndex - INODE_DATA_POINTER_COUNT];
            if (dataBlockIndex < 0) {
                /* Indirect block's pointer was not set up at our index */
                dataBlockIndex = free_bitmap_nextFree(&free_bitmap);
                if (dataBlockIndex < 0) return -1; /* No blocks available */
                free_bitmap_setUsed(&free_bitmap, dataBlockIndex);
                indirect_block.pointers[inodeDataPtrIndex - INODE_DATA_POINTER_COUNT] = dataBlockIndex;
                /* Write the updated indirect block back to disk */
                write_blocks(FILES_DATABLOCK_INDEX + inode->indirectPointer, 1, &indirect_block);
            } else {
                /* Indirect block's pointer is already pointing to a valid block, read it into disk buffer */
                read_from_disk(FILES_DATABLOCK_INDEX + dataBlockIndex, BLOCK_SIZE, diskBuffer);
            }
        }
        /* Disk buffer is now either an empty or partially full block that is pointed to by either a data pointer index
         * in the inode, or a pointer in the inode's indirect block, we can now write to the buffer and save it on disk */
        memcpy(diskBuffer + inodeDataBlockStartIndex, buf, (size_t) bytesToWrite);
        write_blocks(FILES_DATABLOCK_INDEX + dataBlockIndex, 1, diskBuffer);
        /* Increment the fd rw-pointer and keep track of what was written and what's left to write */
        fd->rw_pointer += bytesToWrite;
        bytesLeft -= bytesToWrite;
        bytesWritten += bytesToWrite;
        /* Increment the buffer so that if we loop again, we continue where we left off */
        buf += bytesToWrite;
        /* If the write resulted in an increase in the file-size then record this */
        inode->size = fd->rw_pointer > inode->size ? fd->rw_pointer : inode->size;
    }
    /* We're done with the temporary disk buffer, free it */
    free(diskBuffer);
    /* Write the updated inode table and free_bitmap table back to the disk */
    write_blocks(INODETABLE_DATABLOCK_INDEX, (int) byteToBlock(sizeof(inode_table_t)), inode_table);
    write_blocks(FREE_BITMAP_DATABLOCK_INDEX, (int) byteToBlock(sizeof(free_bitmap_t)), &free_bitmap);
    return bytesWritten;
}

int sfs_fseek(int fileID, int loc) {
    if (fileID < 0 || loc < 0) return -1; /* Invalid parameters! */
    if (fd_table_isFree(fd_table, fileID)) return -1; /* File was closed, can't seek! */
    fd_table->table[fileID].rw_pointer = loc;
    return 0;
}

int sfs_remove(char *file) {
    int inodeIndex = directory_getInodeIndex(root_dir, file);
    if (inodeIndex < 0) return -1; /* File doesn't exist */
    free_bitmap_t freeBitmap;
    read_from_disk(FREE_BITMAP_DATABLOCK_INDEX, BLOCK_SIZE, &freeBitmap);
    inode_t* inode = &inode_table->nodes[inodeIndex];
    /* Close the file descriptor too */
    int fdIndex = fd_table_contains_inode(fd_table, inodeIndex);
    sfs_fclose(fdIndex);
    /* Remove data pointed to by data blocks pointers of the inode */
    for (int i = 0; i < INODE_DATA_POINTER_COUNT; ++i) {
        if (inode->data_pointrs[i] > -1) {
            void* temp = malloc(BLOCK_SIZE);
            memset(temp, 0, BLOCK_SIZE);
            write_blocks(FILES_DATABLOCK_INDEX + inode->data_pointrs[i], 1, temp);
            free(temp);
            free_bitmap_setFree(&freeBitmap, inode->data_pointrs[i]);
        }
    }
    /* Did the inode use an indirect block? */
    if (inode->indirectPointer > -1) {
        /* First we'll delete all the content pointed to by the indirect block */
        indirect_block_t indirect_block;
        read_from_disk(FILES_DATABLOCK_INDEX + inode->indirectPointer, BLOCK_SIZE, &indirect_block);
        for (int i = 0; i < BLOCK_SIZE / sizeof(int); ++i) {
            if (indirect_block.pointers[i] > -1) {
                void* temp = malloc(BLOCK_SIZE);
                memset(temp, 0, BLOCK_SIZE);
                write_blocks(FILES_DATABLOCK_INDEX + indirect_block.pointers[i], 1, temp);
                free(temp);
                free_bitmap_setFree(&freeBitmap, indirect_block.pointers[i]);
            }
        }
        /* Then we delete the indirect block itself too */
        void* temp = malloc(BLOCK_SIZE);
        memset(temp, 0, BLOCK_SIZE);
        write_blocks(FILES_DATABLOCK_INDEX + inode->indirectPointer, 1, temp);
        free(temp);
        free_bitmap_setFree(&freeBitmap, inode->indirectPointer);
    }
    /* Reinitialize that inode to blank */
    inode_init(inode);
    inode_table_setFree(inode_table, inodeIndex);
    /* Find which entry in the directory that file was and reset it */
    int dirIndex = directory_getEntryIndex(root_dir, file);
    /* Reinitialize that directory entry to blank */
    directory_entry_init(&root_dir->entries[dirIndex]);
    directory_setFree(root_dir, dirIndex);
    /* Save the things we updated here on to the disk */
    write_blocks(INODETABLE_DATABLOCK_INDEX, (int) byteToBlock(sizeof(inode_table_t)), inode_table);
    write_blocks(FREE_BITMAP_DATABLOCK_INDEX, (int) byteToBlock(sizeof(free_bitmap_t)), &freeBitmap);
    write_blocks(ROOT_DIR_DATABLOCK_INDEX, (int) byteToBlock(sizeof(directory_t)), root_dir);
    return 0;
}

void FREE_AND_EXIT() {
    free(inode_table);
    free(root_dir);
    free(fd_table);
    exit(0);
}

