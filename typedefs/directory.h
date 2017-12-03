//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_DIRECTORY_H
#define ASSIGNMENT3_DIRECTORY_H

#include "directory_entry.h"

/**
 * @brief Directory data structure
 *
 * @brief Contains an array of directory_entry_t of size INODE_COUNT - 1, since 1 inode
 * is always dedicated to the root directory itself.
 *
 * @brief Contains a char array of the same size, used to indicate which slots are full and
 * which are empty.
 *
 * @brief Also uses a currentIndex integer, to be able to remember the index at which it was previously
 * accessed, in case such functionality is needed.
 *
 * @size 2479 bytes if INODE_COUNT == 100
 */
typedef struct {
    directory_entry_t entries[INODE_COUNT - 1]; /* -1 because 1 inode is for root_dir */
    char free[INODE_COUNT - 1];
    int currentIndex;
} directory_t;

/**
 * @brief Initializes a new directory be calling initialize on all the directory entries, as well as
 * marking every entry as free in the 'free' array and setting currentIndex to 0.
 * @param directory Pointer to directory
 */
void directory_init(directory_t *directory) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        directory_entry_init(&directory->entries[i]);
        /* i + 1, since 0th dir entry's inode will be inode 1 and not 0 (inode 0 = root_dir) */
        directory_entry_setInodeIndex(&directory->entries[i], i + 1);
        directory->free[i] = 1;
        directory->currentIndex = 0;
    }
}

/**
 * @brief Marks the spot at the given index in the directory as used (sets free[index] to 0)
 * @param directory Pointer to directory
 * @param index Index to set used
 */
void directory_setUsed(directory_t *directory, int index) {
    directory->free[index] = 0;
}
/**
 * @brief Marks the spot at the given index in the directory as free (sets free[index] to 1)
 * @param directory Pointer to directory
 * @param index Index to set free
 */
void directory_setFree(directory_t *directory, int index) {
    directory->free[index] = 1;
}

/**
 * @brief Checks if the spot at the given index in the directory is free
 * @param directory Pointer to directory
 * @param index Index to check if free
 * @return 1 if free, 0 if used
 */
int directory_isFree(directory_t *directory, int index) {
    return directory->free[index] == 1;
}

/**
 * @brief Checks if the spot at the given index in the directory is used
 * @param directory Pointer to directory
 * @param index Index to check if used
 * @return 1 if used, 0 if free
 */
int directory_isUsed(directory_t *directory, int index) {
    return directory->free[index] == 0;
}

/**
 * @brief Crawls a directory looking for the next free spot
 * @param directory Pointer to directory
 * @return index of the next free spot, if any, and -1 if no free spots available
 */
int directory_nextFree(directory_t *directory) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (directory_isFree(directory, i)) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Crawls a directory, attempting to search for an entry that has a matching name as the given parameter
 * @param directory Pointer to directory
 * @param name Name of entry to search for
 * @return The I-Node index of the entry if found, -1 if entry with given name doesn't exist
 */
int directory_getInodeIndex(directory_t *directory, const char *name) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (strncmp(directory->entries[i].name, name, strlen(name)) == 0
            && directory->free[i] == 0) {
            return directory->entries[i].inode_index;
        }
    }
    return -1;
}

/**
 * @brief Crawls a directory, attempting to search for an entry that has a matching name as the given parameter
 * @param directory Pointer to directory
 * @param name Name of entry to search for
 * @return The index of the entry within the directory entry array of the directory if found, -1 if entry
 * with given name doesn't exist
 */
int directory_getEntryIndex(directory_t* directory, const char *name) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (strncmp(directory->entries[i].name, name, strlen(name)) == 0
                && directory->free[i] == 0) {
            return i;
        }
    }
    return -1;
}



#endif //ASSIGNMENT3_DIRECTORY_H
