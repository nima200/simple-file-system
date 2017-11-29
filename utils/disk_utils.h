//
// Created by nima on 28/11/17.
//

#include <stdlib.h>
#include <memory.h>
#include "../constants.h"
#include "../disk_emu.h"

#ifndef ASSIGNMENT3_DISK_UTILS_H
#define ASSIGNMENT3_DISK_UTILS_H

size_t byteToBlock(size_t size) {
    return ((size + BLOCK_SIZE - 1) / BLOCK_SIZE);
}

void read_from_disk(int start_address, size_t size, void* buffer) {
    size_t blockCount = byteToBlock(size);
    void* tempBuffer = malloc(blockCount * BLOCK_SIZE);
    memset(tempBuffer, 0, blockCount * BLOCK_SIZE);
    read_blocks(start_address, (int) blockCount, tempBuffer);
    memcpy(buffer, tempBuffer, size);
    free(tempBuffer);
}

#endif //ASSIGNMENT3_DISK_UTILS_H
