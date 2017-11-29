//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_DIRECTORY_ENTRY_H
#define ASSIGNMENT3_DIRECTORY_ENTRY_H

#include <memory.h>
#include "../constants.h"

typedef struct {
    int inode_index;
    char name[MAX_FILE_NAME_LENGTH];
} directory_entry_t;

void directory_entry_init(directory_entry_t* directory_entry) {
    memset(directory_entry->name, 0, MAX_FILE_NAME_LENGTH);
    directory_entry->inode_index = -1;
}

void directory_entry_setName(directory_entry_t* directory_entry, char* name) {
    memcpy(directory_entry->name, name, MAX_FILE_NAME_LENGTH);
}

void directory_entry_setInodeIndex(directory_entry_t *directory_entry, int inode_index) {
    directory_entry->inode_index = inode_index;
}

#endif //ASSIGNMENT3_DIRECTORY_ENTRY_H
