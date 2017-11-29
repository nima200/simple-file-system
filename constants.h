//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_CONSTANTS_H
#define ASSIGNMENT3_CONSTANTS_H

#define BLOCK_SIZE 1024
#define BLOCK_COUNT 1024
/* 1024 - 1 (super) - 8 (inode_table) - 3 (root_dir) - 1 (free_bitmap_table) = 1011 */
#define FILES_DATABLOCK_COUNT 1011
#define INODE_COUNT 100
#define INODE_DATA_POINTER_COUNT 12
#define INODETABLE_BLOCK_COUNT 8
#define MAX_FILE_NAME_LENGTH 20

#define SUPERBLOCK_DATABLOCK_INDEX 0 /* Takes 1 block */
#define INODETABLE_DATABLOCK_INDEX 1 /* Takes 8 blocks */
#define FREE_BITMAP_DATABLOCK_INDEX 9 /* Takes 1 block */
#define ROOT_DIR_DATABLOCK_INDEX 10 /* Takes 3 blocks */
#define FILES_DATABLOCK_INDEX 13 /* Takes 1011 blocks */


#endif //ASSIGNMENT3_CONSTANTS_H
