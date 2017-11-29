//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_INODE_TABLE_H
#define ASSIGNMENT3_INODE_TABLE_H

#include "inode.h"

typedef struct {
    inode_t nodes[INODE_COUNT];
    char free[INODE_COUNT];
} inode_table_t;

void inode_table_init(inode_table_t* inode_table) {
    for (int i = 0; i < INODE_COUNT; ++i) {
        inode_init(&inode_table->nodes[i]);
        inode_table->free[i] = 1;
    }
}

void inode_table_setFree(inode_table_t* inode_table, int index) {
    inode_table->free[index] = 1;
}

void inode_table_setUsed(inode_table_t* inode_table, int index) {
    inode_table->free[index] = 0;
}

int inode_table_isUsed(inode_table_t* inode_table, int index) {
    return inode_table->free[index] == 0;
}

int inode_table_isFree(inode_table_t* inode_table, int index) {
    return inode_table->free[index] == 1;
}

int inode_table_nextFree(inode_table_t* inode_table) {
    for (int i = 0; i < INODE_COUNT; ++i) {
        if (inode_table_isFree(inode_table, i)) {
            return i;
        }
    }
    return -1;
}

#endif //ASSIGNMENT3_INODE_TABLE_H
