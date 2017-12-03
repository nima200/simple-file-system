//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_INODE_TABLE_H
#define ASSIGNMENT3_INODE_TABLE_H

#include "inode.h"

/**
 * @brief Wrapper data structure that holds an array of I-Nodes, as well as a character array that represents
 * the free slots in the table.
 * @size 7300 bytes if INODE_COUNT == 100
 */
typedef struct {
    inode_t nodes[INODE_COUNT];
    char free[INODE_COUNT];
} inode_table_t;

/**
 * @brief Initializes an I-Node table by calling init on all the individual I-Nodes, as well
 * as marking every entry as free in the free character array
 * @param inode_table Pointer to I-Node table
 */
void inode_table_init(inode_table_t* inode_table) {
    for (int i = 0; i < INODE_COUNT; ++i) {
        inode_init(&inode_table->nodes[i]);
        inode_table->free[i] = 1;
    }
}

/**
 * @brief Marks an entry in the I-Node table as free
 * @param inode_table Pointer to I-Node table
 * @param index Index to mark as free
 */
void inode_table_setFree(inode_table_t* inode_table, int index) {
    inode_table->free[index] = 1;
}

/**
 * @brief Marks an entry in the I-Node table as used
 * @param inode_table Pointer to I-Node table
 * @param index Index to mark as used
 */
void inode_table_setUsed(inode_table_t* inode_table, int index) {
    inode_table->free[index] = 0;
}

/**
 * @brief Checks if an entry in the I-Node table is marked as used or not
 * @param inode_table Pointer to I-Node table
 * @param index Index to check if used
 * @return 1 if used, 0 if free
 */
int inode_table_isUsed(inode_table_t* inode_table, int index) {
    return inode_table->free[index] == 0;
}

/**
 * @brief Checks if an entry in the I-Node table is marked as free or not
 * @param inode_table Pointer to I-Node table
 * @param index Index to check if free
 * @return 1 if free, 0 if used
 */
int inode_table_isFree(inode_table_t* inode_table, int index) {
    return inode_table->free[index] == 1;
}

/**
 * @brief Crawls an I-Node table looking for the next free entry, if any
 * @param inode_table Pointer to I-Node table
 * @return Index of next free entry, if any, -1 if none are free
 */
int inode_table_nextFree(inode_table_t* inode_table) {
    for (int i = 0; i < INODE_COUNT; ++i) {
        if (inode_table_isFree(inode_table, i)) {
            return i;
        }
    }
    return -1;
}

#endif //ASSIGNMENT3_INODE_TABLE_H
