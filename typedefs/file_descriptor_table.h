//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_FILE_DESCRIPTOR_TABLE_H
#define ASSIGNMENT3_FILE_DESCRIPTOR_TABLE_H

#include "../constants.h"
#include "file_descriptor.h"

/**
 * @brief Wrapper data structure around file descriptors that puts together an array of them, as well
 * as holding a character array for the free spots, with values 1 for free and 0 for used at any index
 * @size 1683 bytes if INODE_COUNT == 100
 */
typedef struct {
    file_descriptor_t table[INODE_COUNT -1];
    char free[INODE_COUNT -1];
} file_descriptor_table_t;

/**
 * @brief Initializes a file descriptor table by calling fd_init on all entries and marking all entries
 * as free in the free character array
 * @param file_descriptor_table Pointer to file descriptor table
 */
void fd_table_init(file_descriptor_table_t* file_descriptor_table) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        fd_init(&file_descriptor_table->table[i]);
        file_descriptor_table->free[i] = 1;
    }
}

/**
 * @brief Checks if an index at the file descriptor table is free
 * @param file_descriptor_table Pointer to file descriptor table
 * @param index Index to check if free
 * @return 1 if free, 0 if used
 */
int fd_table_isFree(file_descriptor_table_t* file_descriptor_table, int index) {
    return file_descriptor_table->free[index] == 1;
}

/**
 * @brief Checks if an index at the file descriptor table is used
 * @param file_descriptor_table Pointer to file descriptor table
 * @param index Index to check if used
 * @return 1 if used, 0 if free
 */
int fd_table_isUsed(file_descriptor_table_t* file_descriptor_table, int index) {
    return file_descriptor_table->free[index] == 0;
}

/**
 * @brief Marks an index at the file descriptor table as free
 * @param file_descriptor_table Pointer to file descriptor table
 * @param index Index to mark as free
 */
void fd_table_setFree(file_descriptor_table_t* file_descriptor_table, int index) {
    file_descriptor_table->free[index] = 1;
}

/**
 * @brief Marks an index at the file descriptor table as used
 * @param file_descriptor_table Pointer to file descriptor table
 * @param index Index to mark as used
 */
void fd_table_setUsed(file_descriptor_table_t* file_descriptor_table, int index) {
    file_descriptor_table->free[index] = 0;
}

/**
 * @brief Crawls a file descriptor table and looks for the next free spot, if any
 * @param file_descriptor_table Pointer to file descriptor table
 * @return Index of the free entry if any, -1 if none are free
 */
int fd_table_nextFree(file_descriptor_table_t* file_descriptor_table) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (fd_table_isFree(file_descriptor_table, i)) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Crawls a file descriptor table looking for an entry that has a matching I-Node index
 * @param file_descriptor_table Pointer to file descriptor table
 * @param inode_index I-Node Index to search for
 * @return Index of the entry that matches that I-Node index, -1 if no such entry found
 */
int fd_table_contains_inode(file_descriptor_table_t* file_descriptor_table, int inode_index) {
    for (int i = 0; i < INODE_COUNT - 1; ++i) {
        if (file_descriptor_table->table[i].inode_index == inode_index) {
            return i;
        }
    }
    return -1;
}

#endif //ASSIGNMENT3_FILE_DESCRIPTOR_TABLE_H
