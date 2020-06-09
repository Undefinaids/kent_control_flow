//
// Created by antoine on 20/05/2020.
//

#ifndef NEW_CONTROL_FLOW_DOT_H
#define NEW_CONTROL_FLOW_DOT_H

#include "control_flow.h"

typedef enum {
	O_STRICT = 1 << 0,
	O_UNDIRECT = 1 << 1,
	O_DIRECT = 1 << 2
}	mode_graph_t;

typedef enum {
	O_CONDITIONAL = 1 << 0,
	O_NORMAL = 1 << 1,
	O_UNCONDITIONAL = 1 << 2
}	mode_link_t;

typedef struct              old_dot_block_s {
	char                    *id;
	subblock_t              *subblock;
	struct old_dot_block_s  *next;
}                           old_dot_block_t;

typedef struct          dot_link_s {
	const char          *src;
	const char          *dest;
	mode_link_t         mode;
	struct dot_link_s   *next;
}                       dot_link_t;

const char *get_dot_instructions(char **instructions, unsigned int offset, unsigned int size);
void print_graph_blocks(FILE *file, program_t *program, dot_link_t **links);
void dump_models(program_t *program, const char *pathname, mode_graph_t mode);
dot_link_t *push_link(dot_link_t **links, const char *src, const char *dest, mode_link_t mode);
void print_graph_links(FILE *file, dot_link_t *links);
char *has_been_printed(old_dot_block_t *paths, subblock_t *subblock);
old_dot_block_t *push_old_dot_block(old_dot_block_t **dot_blocks, char *id, subblock_t *subblock);

#endif //NEW_CONTROL_FLOW_DOT_H
