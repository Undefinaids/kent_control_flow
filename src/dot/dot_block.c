//
// Created by antoine on 14/05/2020.
//

#include "dot_graph.h"

void dotblock_print(dotblock_t *blocks) {
	if (blocks == NULL)
		LOG("Empty dotblocks\n");
	for (dotblock_t *iterator = blocks; iterator != NULL; iterator = iterator->next) {
		LOG("Dotblock offset %d and size %d\n", iterator->offset, iterator->size);
	}
}

dotblock_t *dotblock_push(dotblock_t *blocks, unsigned int offset, unsigned size) {
	dotblock_t *new_block;

	if ((new_block = malloc(sizeof(dotblock_t))) == NULL) {
		ERROR("Failed to malloc new dotblock");
	}
	new_block->offset= offset;
	new_block->size = size;
	new_block->next = NULL;
	if (blocks == NULL)
		return (new_block);
	for (dotblock_t *iterator = blocks; iterator != NULL; iterator = iterator->next)
		if (iterator->next == NULL)
			iterator->next = new_block;
	return (new_block);
}