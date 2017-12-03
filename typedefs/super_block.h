//
// Created by nima on 28/11/17.
//

#ifndef ASSIGNMENT3_SUPER_BLOCK_H
#define ASSIGNMENT3_SUPER_BLOCK_H

/**
 * @brief Data structure that holds all the necessary information of a superblock
 * @size 80 bytes
 */
typedef struct {
    uint64_t magic;
    int block_size;
    int fs_size;
    int inode_table_length;
    int root_dir_inode_index;
} super_block_t;

/**
 * @brief Initializes a superblock by setting all fields as 0 (if unsigned int) or -1 (if int) showing that
 * they haven't been set yet
 * @param super_block Pointer to superblock
 */
void super_block_init(super_block_t* super_block) {
    super_block->magic = 0;
    super_block->block_size = -1;
    super_block->inode_table_length = -1;
    super_block->fs_size = -1;
    super_block->root_dir_inode_index = -1;
}

#endif //ASSIGNMENT3_SUPER_BLOCK_H
