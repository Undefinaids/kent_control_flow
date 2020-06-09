//
// Created by antoine on 21/05/2020.
//

#include "dot.h"

old_dot_block_t *init_old_dot_block(char *id, subblock_t *subblock) {
	old_dot_block_t *old_dot_block;

	if ((old_dot_block = malloc(sizeof(old_dot_block_t))) == NULL) {
		ERROR("Failed to allocate old dot block");
		return (NULL);
	}
	old_dot_block->subblock = subblock;
	old_dot_block->id = id;
	old_dot_block->next = NULL;
	return (old_dot_block);
}

old_dot_block_t *push_old_dot_block(old_dot_block_t **dot_blocks, char *id, subblock_t *subblock) {
	old_dot_block_t *old_dot_block;
	old_dot_block_t *iterator;

	if ((old_dot_block = init_old_dot_block(id, subblock)) == NULL) {
		return (NULL);
	}
	if (*dot_blocks == NULL) {
		*dot_blocks = old_dot_block;
		return (old_dot_block);
	}
	for (iterator = *dot_blocks; iterator->next != NULL; iterator = iterator->next);
	iterator->next = old_dot_block;
	return (old_dot_block);
}

char *has_been_printed(old_dot_block_t *paths, subblock_t *subblock) {
	for (old_dot_block_t *iterator = paths; iterator != NULL; iterator = iterator->next) {
		if (iterator->subblock == subblock) {
			LOG("Old dot block match found\n");
			return (iterator->id);
		}
	}
	return (NULL);
}