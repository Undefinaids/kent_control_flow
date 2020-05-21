//
// Created by antoine on 19/05/2020.
//

#include "control_flow.h"

subblock_t *init_subblock(unsigned int offset) {
	subblock_t *subblock;

	if ((subblock = malloc(sizeof(subblock_t))) == NULL) {
		ERROR("Failed to malloc a new subblock_t");
		return (NULL);
	}
	subblock->offset = offset;
	subblock->size = 0;
	subblock->counter = 0;
	subblock->parent = NULL;
	subblock->next = NULL;
	subblock->conditional = NULL;
	return (subblock);
}