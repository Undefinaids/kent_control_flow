//
// Created by antoine on 21/05/2020.
//

#ifndef NEW_CONTROL_FLOW_OLD_PATH_H
#define NEW_CONTROL_FLOW_OLD_PATH_H

#include "control_flow.h"

typedef struct subblock_s subblock_t;

typedef struct          old_path_s {
	unsigned int        offset;
	subblock_t          *subblock;
	struct old_path_s   *next;
}                       old_path_t;

subblock_t *has_been_visited(old_path_t *paths, unsigned int offset);
old_path_t *push_old_path(old_path_t **paths, unsigned int offset, subblock_t *subblock);

#endif //NEW_CONTROL_FLOW_OLD_PATH_H
