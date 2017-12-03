//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_FREE_BITMAP_H
#define ASSIGNMENT3_FREE_BITMAP_H

#include "../constants.h"

/**
 * @brief Wrapper data structure around a character array that mimics a free bitmap for data-blocks
 * @size 1011 bytes if FILE_DATABLOCK_COUNT == 1011
 */
typedef struct {
    char bits[FILES_DATABLOCK_COUNT];
} free_bitmap_t;

/**
 * @brief Initializes a bitmap by marking all entries as free
 * @param free_bitmap Pointer to bitmap
 */
void free_bitmap_init(free_bitmap_t *free_bitmap) {
    for (int i = 0; i < FILES_DATABLOCK_COUNT; ++i) {
        free_bitmap->bits[i] = 1;
    }
}

/**
 * @brief Marks an entry in a bitmap table as used
 * @param free_bitmap Pointer to bitmap table
 * @param index Index to mark as used
 */
void free_bitmap_setUsed(free_bitmap_t* free_bitmap, int index) {
    free_bitmap->bits[index] = 0;
}

/**
 * @brief Marks an entry in a bitmap table as free
 * @param free_bitmap Pointer to bitmap table
 * @param index Index to mark as free
 */
void free_bitmap_setFree(free_bitmap_t* free_bitmap, int index) {
    free_bitmap->bits[index] = 1;
}

/**
 * @brief Checks if the entry in a bitmap table at the given index is marked as free
 * @param free_bitmap Pointer to bitmap table
 * @param index Index to check if free
 * @return 1 if index is marked as free, 0 if used
 */
int free_bitmap_isFree(free_bitmap_t* free_bitmap, int index) {
    return free_bitmap->bits[index] == 1;
}

/**
 * @brief Checks if the entry in a bitmap table at the given index is marked as used
 * @param free_bitmap Pointer to bitmap table
 * @param index Index to check if used
 * @return 1 if index is marked as used, 0 if free
 */
int free_bitmap_isUsed(free_bitmap_t* free_bitmap, int index) {
    return free_bitmap->bits[index] == 0;
}

/**
 * @brief Crawls a bitmap table looking for the next entry marked as free
 * @param free_bitmap Pointer to bitmap table
 * @return Index of next free entry if any, -1 if none found
 */
int free_bitmap_nextFree(free_bitmap_t* free_bitmap) {
    for (int i = 0; i < FILES_DATABLOCK_COUNT; ++i) {
        if (free_bitmap_isFree(free_bitmap, i)) {
            return i;
        }
    }
    return -1;
}

#endif //ASSIGNMENT3_FREE_BITMAP_H
