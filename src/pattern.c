//
// Created by antoine on 19/05/2020.
//

#include "pattern.h"
#include "utils.h"

const pattern_t patterns[NB_PATTERN] = {
		// Disable goto
		// {"goto block", &goto_action},
		{"exit block", &exit_action},
		{"branch conditional", &branch_action}
};

subblock_t *exit_action(__attribute__((unused)) program_t *program,
		__attribute__((unused)) block_t *block, subblock_t *subblock) {
	return (subblock);
}

subblock_t *goto_action(program_t *program, block_t *block, subblock_t *subblock) {
	char *instruction = program->instructions[subblock->offset + subblock->size];
	char *label_name = extract_first_label(instruction + 10);
	unsigned int offset_label;
	subblock_t *old_path;

	offset_label = get_offset_by_label_name(block->labels, label_name);
	if (offset_label != 0) {
		LOG("OFFSET FOUND\n");
		//Check if path has already been visited (avoid duplicate)
		old_path = has_been_visited(block->old_paths, offset_label);
		if (old_path == NULL)
			subblock->next = parse_subblocks(program, block, offset_label, block->offset + block->size);
		else
			subblock->next = old_path;
	} else {
		LOG("OFFSET NOT FOUND\n");
	}
	return (subblock);
}

subblock_t *branch_action(program_t *program, block_t *block, subblock_t *subblock) {
	char *instruction = program->instructions[subblock->offset + subblock->size];
	char *label_name = extract_first_label(instruction + 26);
	unsigned int offset_label;
	subblock_t *old_path;

	offset_label = get_offset_by_label_name(block->labels, label_name);
	if (offset_label != 0) {
		old_path = has_been_visited(block->old_paths, offset_label);
		if (old_path == NULL)
			subblock->conditional = parse_subblocks(program, block, offset_label, block->offset + block->size);
		else
			subblock->conditional = old_path;
	}
	old_path = has_been_visited(block->old_paths, subblock->offset + subblock->size + 1);
	if (old_path == NULL)
		subblock->next = parse_subblocks(program, block, subblock->offset + subblock->size + 1, block->offset + block->size);
	else
		subblock->next = old_path;
	return (subblock);
}

subblock_t *empty_action(program_t *program, block_t *block, subblock_t *subblock) {
	LOG("Instruction -> %s\n", program->instructions[subblock->offset + subblock->size]);
	//ERROR("No action related to the pattern");
	return (NULL);
}
