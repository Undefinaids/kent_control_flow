//
// Created by antoine on 19/05/2020.
//

#ifndef NEW_CONTROL_FLOW_CONTROL_FLOW_H
#define NEW_CONTROL_FLOW_CONTROL_FLOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

typedef struct          subblock_s {
	unsigned int        offset;
	unsigned int        size;
	unsigned int        counter;
	struct subblock_s   *parent;
	struct subblock_s   *next;
	struct subblock_s   *conditional;
}                       subblock_t;

typedef struct      label_s {
	const char      *name;
	unsigned int    offset;
	struct label_s  *next;
}                   label_t;

typedef struct      block_s {
	unsigned int    offset;
	unsigned int    size;
	unsigned int    counter;
	label_t         *labels;
	subblock_t      *subblocks;
	struct block_s  *next;
}                   block_t;

typedef struct      program_s {
	char            **instructions;
	unsigned int    counter;
	block_t         *blocks;
}                   program_t;

// Subblocks function
subblock_t *init_subblock(unsigned int offset);

// Labels function
label_t *init_label(const char *name, unsigned int offset);
label_t *push_label(label_t **labels, const char *name, unsigned int offset);
unsigned int get_offset_by_label_name(label_t *label, const char *name);

// Blocks function
block_t *init_block(unsigned int offset);
block_t *push_block(block_t **blocks, unsigned int offset);
int count_block(block_t *start);
unsigned int extract_labels_block(program_t *program, block_t *block);

// Main functions
int parser(program_t *program);
subblock_t *parse_subblocks(program_t *program, block_t *block, unsigned int offset, unsigned int limit);

#endif //NEW_CONTROL_FLOW_CONTROL_FLOW_H
