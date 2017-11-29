//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_FREE_BITMAP_H
#define ASSIGNMENT3_FREE_BITMAP_H

#include "../constants.h"

typedef struct {
    char bits[FILES_DATABLOCK_COUNT];
} free_bitmap_t;

void free_bitmap_init(free_bitmap_t *free_bitmap) {
    for (int i = 0; i < FILES_DATABLOCK_COUNT; ++i) {
        free_bitmap->bits[i] = 1;
    }
}

void free_bitmap_setUsed(free_bitmap_t* free_bitmap, int index) {
    free_bitmap->bits[index] = 0;
}

void free_bitmap_setFree(free_bitmap_t* free_bitmap, int index) {
    free_bitmap->bits[index] = 1;
}

int free_bitmap_isFree(free_bitmap_t* free_bitmap, int index) {
    return free_bitmap->bits[index] == 1;
}

int free_bitmap_isUsed(free_bitmap_t* free_bitmap, int index) {
    return free_bitmap->bits[index] == 0;
}

int free_bitmap_nextFree(free_bitmap_t* free_bitmap) {
    for (int i = 0; i < FILES_DATABLOCK_COUNT; ++i) {
        if (free_bitmap_isFree(free_bitmap, i)) {
            return i;
        }
    }
    return -1;
}

#endif //ASSIGNMENT3_FREE_BITMAP_H
