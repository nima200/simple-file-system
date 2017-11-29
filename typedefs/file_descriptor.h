//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_FILE_DESCRIPTOR_H
#define ASSIGNMENT3_FILE_DESCRIPTOR_H

#include "inode.h"

typedef struct {
    int inode_index;
    inode_t* inode;
    int rw_pointer;
} file_descriptor_t;

void fd_init(file_descriptor_t* file_descriptor) {
    file_descriptor->inode_index = -1;
    file_descriptor->rw_pointer = -1;
    file_descriptor->inode = NULL;
}

#endif //ASSIGNMENT3_FILE_DESCRIPTOR_H
