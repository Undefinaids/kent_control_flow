//
// Created by antoine on 21/05/2020.
//

#ifndef NEW_CONTROL_FLOW_OLD_PATH_H
#define NEW_CONTROL_FLOW_OLD_PATH_H

#include "control_flow.h"

typedef struct          old_path_s {
	unsigned int        instruction;
	subblock_t          *subblock;
	struct old_path_s   *next;
}                       old_path_t;

#endif //NEW_CONTROL_FLOW_OLD_PATH_H
