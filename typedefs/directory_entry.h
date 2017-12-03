//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_DIRECTORY_ENTRY_H
#define ASSIGNMENT3_DIRECTORY_ENTRY_H

#include <memory.h>
#include "../constants.h"

/**
 * @brief A directory entry data structure that is composed of an I-Node Index being the corresponding index of the
 * respective inode of this entry, and a character array as the name of the entry, capped at MAX_FILE_NAME_LENGTH as size
 * @size 24 bytes if MAX_FILE_NAME_LENGTH == 20
 */
typedef struct {
    int inode_index;
    char name[MAX_FILE_NAME_LENGTH];
} directory_entry_t;

/**
 * @brief Initializes a directory entry by setting all bytes in the name array to 0, and marking the I-Node index as -1
 * to show that it has not been set yet.
 * @param directory_entry Pointer to the directory entry to initialize
 */
void directory_entry_init(directory_entry_t* directory_entry) {
    memset(directory_entry->name, 0, MAX_FILE_NAME_LENGTH);
    directory_entry->inode_index = -1;
}

/**
 * @brief Sets the name of a directory entry as the first MAX_FILE_NAME_LENGTH characters of the name parameter
 * @param directory_entry Pointer to the directory entry
 * @param name Desired name of entry
 */
void directory_entry_setName(directory_entry_t* directory_entry, char* name) {
    memcpy(directory_entry->name, name, MAX_FILE_NAME_LENGTH);
}

/**
 * @brief Sets the I-Node Index of a directory entry
 * @param directory_entry Pointer to the directory entry
 * @param inode_index I-Node Index to assign to this entry
 */
void directory_entry_setInodeIndex(directory_entry_t *directory_entry, int inode_index) {
    directory_entry->inode_index = inode_index;
}

#endif //ASSIGNMENT3_DIRECTORY_ENTRY_H
