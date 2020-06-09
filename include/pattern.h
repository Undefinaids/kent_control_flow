//
// Created by antoine on 19/05/2020.
//

#ifndef NEW_CONTROL_FLOW_PATTERN_H
#define NEW_CONTROL_FLOW_PATTERN_H

#include "control_flow.h"

#define NB_PATTERN 2

typedef struct      pattern_s {
	const char      *label;
	subblock_t      *(*action)(program_t *program, block_t *block, subblock_t *subblock);
}                   pattern_t;

extern const pattern_t patterns[NB_PATTERN];

subblock_t *empty_action(program_t *program, block_t *block, subblock_t *subblock);
subblock_t *exit_action(program_t *program, block_t *block, subblock_t *subblock);
subblock_t *branch_action(program_t *program, block_t *block, subblock_t *subblock);
subblock_t *goto_action(program_t *program, block_t *block, subblock_t *subblock);

#endif //NEW_CONTROL_FLOW_PATTERN_H
