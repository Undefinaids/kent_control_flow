//
// Created by antoine on 19/05/2020.
//

#include "control_flow.h"
#include "pattern.h"

int parse_blocks(program_t *program) {
	int i = 0;
	block_t *previous_block = NULL;

	for (i = 0; program->instructions[i] != NULL; ++i) {
		 if (strncmp(program->instructions[i], "translation block start", 23) == 0) {
		 	//New block detected
		 	if (previous_block != NULL)
		 		previous_block->size = i - previous_block->offset;
		 	previous_block = push_block(&program->blocks, i);
		 }
	}
	if (previous_block != NULL)
		previous_block->size = i - previous_block->offset;
	return (count_block(program->blocks));
}

subblock_t *parse_subblocks(program_t *program, block_t *block, unsigned int offset, unsigned int limit) {
	subblock_t *subblock = init_subblock(offset);
	subblock_t *result;
	unsigned int i = 0;

	// MAYBE REMOVE SUBBLOCK PARENT ?
	for (i = subblock->offset; program->instructions[i] != NULL && i < limit; ++i) {
		LOG("%d | %s\n", offset, program->instructions[i]);
		for (unsigned int j = 0; j < NB_PATTERN; ++j) {
			if (strncmp(program->instructions[i], patterns[j].label, strlen(patterns[j].label)) == 0) {
				subblock->size = i - subblock->offset;
				result = patterns[j].action(program, block, subblock);
				push_old_path(&block->old_paths, offset, result);
				return (result);
			}
		}
	}
	subblock->size = i - subblock->offset;
	return (subblock);
}

int parser(program_t *program) {
	if (parse_blocks(program) == 0) {
		LOG("No block found in instructions\n");
		return (0);
	}
	for (block_t *block = program->blocks; block != NULL; block = block->next) {
		LOG("Block offset %d and block size %d\n",block->offset, block->size);
		extract_labels_block(program, block);
		block->subblocks = parse_subblocks(program, block, block->offset, block->offset + block->size);
		optimize_tree(block, block->subblocks);
	}
	return (0);
}

