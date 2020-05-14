//
// Created by antoine on 14/05/2020.
//

#include "dot_graph.h"

void dotlink_print(dotlink_t *links) {
	if (links == NULL)
		LOG("Empty dotlinks\n");
	for (dotlink_t *iterator = links; iterator != NULL; iterator = iterator->next) {
		LOG("Dotlink offset %d and size %d\n", iterator->offset, iterator->size);
	}
}

dotlink_t *dotlink_push(dotlink_t *links, dotblock_t *src, dotblock_t *dest, color_t color) {
	dotlink_t *new_link;

	if ((new_link = malloc(sizeof(dotlink_t))) == NULL) {
		ERROR("Failed to malloc new dotlink");
	}
	new_link->src = src;
	new_link->dest = dest;
	new_link->color = color;
	new_link->next = NULL;
	if (links == NULL)
		return (new_link);
	for (dotlink_t *iterator = links; iterator != NULL; iterator = iterator->next)
		if (iterator->next == NULL)
			iterator->next = new_link;
	return (new_link);
}