//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_INDIRECT_BLOCK_H
#define ASSIGNMENT3_INDIRECT_BLOCK_H

#include "../constants.h"

/**
 * @brief Wrapper data structure around an array of integers that essentially act somewhat as pointers to other data
 * blocks. The actual way it works is that these integers would hold the indices of those data blocks, from a specific
 * offset which is FILES_DATABLOCK_INDEX in this assignment
 * @size 256 bytes if BLOCK_SIZE is 1024
 */
typedef struct {
    int pointers[BLOCK_SIZE / sizeof(int)];
} indirect_block_t;

/**
 * @brief Initializes an indirect block by setting all 256 entries to -1, to show none are being used
 * @param indirect_block Pointer to indirect block
 */
void indirect_block_init(indirect_block_t* indirect_block) {
    for (int i = 0; i < BLOCK_SIZE / sizeof(int); ++i) {
        indirect_block->pointers[i] = -1;
    }
}

/**
 * @brief Checks if an entry in the indirect block, at the given index, is free or not
 * @param indirect_block Pointer to indirect block
 * @param index Index to check if free
 * @return 1 if entry is free, 0 if used
 */
int indirect_block_isFree(indirect_block_t* indirect_block, int index) {
    return indirect_block->pointers[index] == -1;
}

/**
 * @brief Crawls an indirect block, looking for the next free spot, if any
 * @param indirect_block Pointer to indirect block
 * @return Index of next free entry, if any. -1 if none found.
 */
int indirect_block_nextFree(indirect_block_t* indirect_block) {
    for (int i = 0; i < BLOCK_SIZE / sizeof(int); ++i) {
        if (indirect_block_isFree(indirect_block, i)) {
            return i;
        }
    }
    return -1;
}
#endif //ASSIGNMENT3_INDIRECT_BLOCK_H
