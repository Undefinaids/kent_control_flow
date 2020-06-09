//
// Created by antoine on 21/05/2020.
//

#include "old_path.h"

old_path_t *init_old_path(unsigned int offset, subblock_t *subblock) {
	old_path_t *old_path;

	if ((old_path = malloc(sizeof(old_path_t))) == NULL) {
		ERROR("Failed to allocate old path");
		return (NULL);
	}
	old_path->subblock = subblock;
	old_path->offset = offset;
	old_path->next = NULL;
	return (old_path);
}

old_path_t *push_old_path(old_path_t **paths, unsigned int offset, subblock_t *subblock) {
	old_path_t *old_path;
	old_path_t *iterator;

	if ((old_path = init_old_path(offset, subblock)) == NULL) {
		return (NULL);
	}
	if (*paths == NULL) {
		*paths = old_path;
		return (old_path);
	}
	for (iterator = *paths; iterator->next != NULL; iterator = iterator->next);
	iterator->next = old_path;
	return (old_path);
}

subblock_t *has_been_visited(old_path_t *paths, unsigned int offset) {
	for (old_path_t *iterator = paths; iterator != NULL; iterator = iterator->next) {
		if (iterator->offset == offset) {
			LOG("Old path match found\n");
			return (iterator->subblock);
		}
	}
	return (NULL);
}