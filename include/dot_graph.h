//
// Created by antoine on 13/05/2020.
//

#ifndef KENT_CONTROL_FLOW_DOT_GRAPH_H
#define KENT_CONTROL_FLOW_DOT_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

typedef enum {
	true,
	false
} bool;

typedef enum {
	O_STRICT = 1 << 0,
	O_UNDIRECT = 1 << 1
}	mode_graph_t;

typedef enum {
	RED,
	GREEN,
	BLUE
}	color_t;

typedef struct          dotblock_s {
	unsigned int        offset;
	unsigned int        size;
	struct dotblock_s * next;
}                       dotblock_t;

typedef struct          dotlink_s {
	dotblock_t *        src;
	dotblock_t *        dest;
	color_t             color;
	struct dotlink_s *  next;
}                       dotlink_t;

typedef struct  dotgraph_s  {
	bool        strict;
	bool        undirect;
	dotblock_t *blocks;
	dotlink_t  *links;
}               dotgraph_t;

dotgraph_t *dotgraph_init(mode_graph_t mode);
void dotblock_print(dotblock_t *blocks);
dotblock_t *dotblock_push(dotblock_t *blocks, unsigned int offset, unsigned size);
void dotlink_print(dotlink_t *links);
dotlink_t *dotlink_push(dotlink_t *links, dotblock_t *src, dotblock_t *dest, color_t color);

#endif //KENT_CONTROL_FLOW_DOT_GRAPH_H
