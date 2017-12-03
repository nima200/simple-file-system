//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_INODE_H
#define ASSIGNMENT3_INODE_H

#include "../constants.h"

/**
 * @brief I-Node data structure that holds all meta-data information for a file or directory, as well as pointers to the
 * data-blocks that contain the actual contents of the file
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

/**
 * @brief Initializes an I-Node
 * @param inode Pointer to I-Node
 */
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
