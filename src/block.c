//
// Created by antoine on 19/05/2020.
//

#include "control_flow.h"
#include "utils.h"

block_t *init_block(unsigned int offset) {
	block_t *block;

	if ((block = malloc(sizeof(block_t))) == NULL) {
		ERROR("Failed to malloc a new block_t");
		return (NULL);
	}
	block->offset = offset;
	block->size = 0;
	block->counter = 0;
	block->labels = NULL;
	block->subblocks = NULL;
	block->next = NULL;
	return (block);
}

block_t *push_block(block_t **blocks, unsigned int offset) {
	block_t *block;
	block_t *iterator;

	if ((block = init_block(offset)) == NULL) {
		return (NULL);
	}
	if (*blocks == NULL) {
		*blocks = block;
		return (block);
	}
	for (iterator = *blocks; iterator->next != NULL; iterator = iterator->next);
	iterator->next = block;
	return (block);
}

unsigned int extract_labels_block(program_t *program, block_t *block) {
	unsigned int limit = block->offset + block->size;
	unsigned int nb_labels = 0;

	for (unsigned int i = block->offset; program->instructions[i] != NULL && i < limit; ++i) {
		if (strncmp(program->instructions[i], "set label", 9) == 0) {
			push_label(&block->labels, epur_str(program->instructions[i] + 9), i);
			++nb_labels;
		}
	}
	return (nb_labels);
}

int count_block(block_t *start) {
	int i = 0;
	for (block_t *iterator = start; iterator != NULL; ++i, iterator = iterator->next);
	return (i);
}