//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_FILE_DESCRIPTOR_TABLE_H
#define ASSIGNMENT3_FILE_DESCRIPTOR_TABLE_H

#include "../constants.h"
#include "file_descriptor.h"

typedef struct {
    file_descriptor_t table[INODE_COUNT -1];
    char free[INODE_COUNT -1];
} file_descriptor_table_t;

void fd_table_init(file_descriptor_table_t* file_descriptor_table) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        fd_init(&file_descriptor_table->table[i]);
        file_descriptor_table->free[i] = 1;
    }
}

int fd_table_isFree(file_descriptor_table_t* file_descriptor_table, int index) {
    return file_descriptor_table->free[index] == 1;
}

int fd_table_isUsed(file_descriptor_table_t* file_descriptor_table, int index) {
    return file_descriptor_table->free[index] == 0;
}

void fd_table_setFree(file_descriptor_table_t* file_descriptor_table, int index) {
    file_descriptor_table->free[index] = 1;
}

void fd_table_setUsed(file_descriptor_table_t* file_descriptor_table, int index) {
    file_descriptor_table->free[index] = 0;
}

int fd_table_nextFree(file_descriptor_table_t* file_descriptor_table) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (fd_table_isFree(file_descriptor_table, i)) {
            return i;
        }
    }
    return -1;
}

int fd_table_contains_inode(file_descriptor_table_t* file_descriptor_table, int inode_index) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (file_descriptor_table->table[i].inode_index == inode_index) {
            return i;
        }
    }
    return -1;
}

#endif //ASSIGNMENT3_FILE_DESCRIPTOR_TABLE_H
