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

/**
 * @brief Helper function that wraps around the read_blocks method to correctly copy the exact
 * amount of bytes from the respective number of blocks that those bytes fall into, into the
 * buffer provided.
 * @note It is important to use this function when reading from disk as opposed to using the read_blocks
 * method directly, because if you attempt to read x blocks into a data structure that does not fully fill up those blocks,
 * you'll end up overflowing data
 * @param start_address Index of block to start reading from
 * @param size Number of bytes to read
 * @param buffer Buffer to read into
 */
void read_from_disk(int start_address, size_t size, void* buffer) {
    size_t blockCount = byteToBlock(size);
    void* tempBuffer = malloc(blockCount * BLOCK_SIZE);
    memset(tempBuffer, 0, blockCount * BLOCK_SIZE);
    read_blocks(start_address, (int) blockCount, tempBuffer);
    memcpy(buffer, tempBuffer, size);
    free(tempBuffer);
}

#endif //ASSIGNMENT3_DISK_UTILS_H
