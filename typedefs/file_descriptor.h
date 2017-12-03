//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_FILE_DESCRIPTOR_H
#define ASSIGNMENT3_FILE_DESCRIPTOR_H

#include "inode.h"

/**
 * @brief Data structure that holds a file descriptor and relevant information.
 * @brief Holds a pointer to the I-Node for the file, if any, as well as the index
 * of the I-Node within the I-Node table data structure. Additionally, holds a read/write
 * pointer indicating the location at which the file was last accessed as is currently at.
 * @size 16 bytes
 */
typedef struct {
    int inode_index;
    inode_t* inode;
    int rw_pointer;
} file_descriptor_t;

/**
 * @brief Initializes a file descriptor by setting the I-Node index and read/write pointer to -1
 * to show that they have not been set, as well as setting NULL for the I-Node pointer.
 * @param file_descriptor Pointer to file descriptor
 */
void fd_init(file_descriptor_t* file_descriptor) {
    file_descriptor->inode_index = -1;
    file_descriptor->rw_pointer = -1;
    file_descriptor->inode = NULL;
}

#endif //ASSIGNMENT3_FILE_DESCRIPTOR_H
