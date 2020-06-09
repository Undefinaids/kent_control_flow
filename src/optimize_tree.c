//
// Created by antoine on 25/05/2020.
//

#include "control_flow.h"

void compare_subblocks(subblock_t *subblock, subblock_t *to_compare) {
	if (subblock->offset < to_compare->offset && to_compare->offset < subblock->offset + subblock->size) {
		subblock->size = to_compare->offset - subblock->offset - 1;
		subblock->next = to_compare;
		subblock->conditional = NULL;
	}
	if (to_compare->next)
		compare_subblocks(subblock, to_compare->next);
	if (to_compare->conditional)
		compare_subblocks(subblock, to_compare->conditional);
}

void optimize_tree(block_t *block, subblock_t *entry) {
	compare_subblocks(entry, block->subblocks);
	if (entry->next)
		optimize_tree(block, entry->next);
	if (entry->conditional)
		optimize_tree(block, entry->conditional);
}