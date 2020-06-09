//
// Created by antoine on 20/05/2020.
//

#include "control_flow.h"
#include "dot.h"

char *generate_id(char *parent_id, int new_id) {
	int i = 1;
	char *result;
	int length = 0;

	for (int tmp = new_id; tmp >= 10; tmp /= 10, i++);
	length = i + 1;
	if (parent_id)
		length += strlen(parent_id);
	if ((result = calloc(sizeof(char), length)) == NULL) {
		ERROR("Failed to generate id of subblock");
		return (NULL);
	}
	if (parent_id != NULL)
		sprintf(result, "%s%d", parent_id, new_id);
	else
		sprintf(result, "%d", new_id);
	return (result);
}

void print_graph_subblocks(FILE *file, program_t *program,
		old_dot_block_t *old_blocks, dot_link_t **links, char *id, subblock_t *subblock) {
	char *child_id;

	fprintf(file,"%s [label=\"%s\" shape=\"box\"]\n", id,
			get_dot_instructions(program->instructions, subblock->offset, subblock->size + 1));
	push_old_dot_block(&old_blocks, id, subblock);
	if (subblock->next) {
		if ((child_id = has_been_printed(old_blocks, subblock->next)) != NULL) {
			push_link(links, id, child_id, subblock->conditional ? O_NORMAL : O_UNCONDITIONAL);
		} else {
			child_id = generate_id(id, 0);
			push_link(links, id, child_id, subblock->conditional ? O_NORMAL : O_UNCONDITIONAL);
			print_graph_subblocks(file, program, old_blocks, links, child_id, subblock->next);
		}
	}
	if (subblock->conditional) {
		if ((child_id = has_been_printed(old_blocks, subblock->conditional)) != NULL) {
			push_link(links, id, child_id, O_CONDITIONAL);
		} else {
			child_id = generate_id(id, 1);
			push_link(links, id, child_id, O_CONDITIONAL);
			print_graph_subblocks(file, program, old_blocks, links, child_id, subblock->conditional);
		}
	}
}

void print_graph_blocks(FILE *file, program_t *program, dot_link_t **links) {
	int i = 0;
	old_dot_block_t *old_dot_blocks;

	for (block_t *block = program->blocks; block != NULL; block = block->next, ++i) {
		old_dot_blocks = NULL;
		if (block->subblocks)
			print_graph_subblocks(file, program, old_dot_blocks,
					links, generate_id(NULL, i), block->subblocks);
	}
}