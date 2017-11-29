//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_DIRECTORY_H
#define ASSIGNMENT3_DIRECTORY_H

#include "directory_entry.h"

typedef struct {
    directory_entry_t entries[INODE_COUNT - 1]; /* -1 because 1 inode is for root_dir */
    char free[INODE_COUNT - 1];
} directory_t;

void directory_init(directory_t *directory) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        directory_entry_init(&directory->entries[i]);
        /* i + 1, since 0th dir entry's inode will be inode 1 and not 0 (inode 0 = root_dir) */
        directory_entry_setInodeIndex(&directory->entries[i], i + 1);
        directory->free[i] = 1;
    }
}

void directory_setUsed(directory_t *directory, int index) {
    directory->free[index] = 0;
}

void directory_setFree(directory_t *directory, int index) {
    directory->free[index] = 1;
}

int directory_isFree(directory_t *directory, int index) {
    return directory->free[index] == 1;
}

int directory_isUsed(directory_t *directory, int index) {
    return directory->free[index] == 0;
}

int directory_nextFree(directory_t *directory) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (directory_isFree(directory, i)) {
            return i;
        }
    }
    return -1;
}

int directory_contains_name(directory_t *directory, const char *name) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (strncmp(directory->entries[i].name, name, strlen(name)) == 0
            && directory->free[i] == 0) {
            return directory->entries[i].inode_index;
        }
    }
    return -1;
}


#endif //ASSIGNMENT3_DIRECTORY_H
