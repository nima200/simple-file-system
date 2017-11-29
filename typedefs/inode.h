//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_INODE_H
#define ASSIGNMENT3_INODE_H

#include "../constants.h"

/**
 * @size 72 bytes: 6*4 + 12*4
 * */
typedef struct {
    int mode;
    int link_count;
    int uid;
    int gid;
    int size;
    int data_pointrs[INODE_DATA_POINTER_COUNT];
    int indirectPointer;
} inode_t;

void inode_init(inode_t *inode) {
    inode->mode = 0;
    inode->link_count = 1;
    inode-> uid = 0;
    inode->gid = 0;
    inode->size = 0;
    for (int i = 0; i < INODE_DATA_POINTER_COUNT; ++i) {
        inode->data_pointrs[i] = -1;
    }
    inode->indirectPointer = -1;
}

#endif //ASSIGNMENT3_INODE_H
