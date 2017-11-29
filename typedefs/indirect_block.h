//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_INDIRECT_BLOCK_H
#define ASSIGNMENT3_INDIRECT_BLOCK_H

#include "../constants.h"

/* Can store at most 256 pointers to other data blocks */
typedef struct {
    int pointers[BLOCK_SIZE / sizeof(int)];
} indirect_block_t;

void indirect_block_init(indirect_block_t* indirect_block) {
    for (int i = 0; i < BLOCK_SIZE / sizeof(int); ++i) {
        indirect_block->pointers[i] = -1;
    }
}

int indirect_block_isFree(indirect_block_t* indirect_block, int index) {
    return indirect_block->pointers[index] == -1;
}

int indirect_block_nextFree(indirect_block_t* indirect_block) {
    for (int i = 0; i < BLOCK_SIZE / sizeof(int); ++i) {
        if (indirect_block_isFree(indirect_block, i)) {
            return i;
        }
    }
    return -1;
}

int indirect_block_hasSpace(indirect_block_t* indirect_block, int num_block) {
    int freeBlockCount = 0;
    for (int i = 0; i < BLOCK_SIZE / sizeof(int); ++i) {
        if (indirect_block_isFree(indirect_block, i)) {
            freeBlockCount++;
        }
        if (freeBlockCount >= num_block) {
            return 1;
        }
    }
    return 0;
}
#endif //ASSIGNMENT3_INDIRECT_BLOCK_H
