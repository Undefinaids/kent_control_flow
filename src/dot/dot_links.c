//
// Created by antoine on 21/05/2020.
//

#include <stdlib.h>

#include "error.h"
#include "dot.h"

dot_link_t *init_link(const char *src, const char *dest, mode_link_t mode) {
	dot_link_t *link;

	if ((link = malloc(sizeof(dot_link_t))) == NULL) {
		ERROR("Failed to malloc a new block_t");
		return (NULL);
	}
	link->src = src;
	link->dest = dest;
	link->mode = mode;
	link->next = NULL;
	return (link);
}

dot_link_t *push_link(dot_link_t **links, const char *src, const char *dest, mode_link_t mode) {
	dot_link_t *link;
	dot_link_t *iterator;

	if ((link = init_link(src, dest, mode)) == NULL) {
		return (NULL);
	}
	if (*links == NULL) {
		*links = link;
		return (link);
	}
	for (iterator = *links; iterator->next != NULL; iterator = iterator->next);
	iterator->next = link;
	return (link);
}

void print_graph_links(FILE *file, dot_link_t *links) {
	char *color;
	for (dot_link_t *link = links; link != NULL; link = link->next) {
		if (link->mode & O_UNCONDITIONAL)
			color = strdup("black");
		else
			color = link->mode & O_CONDITIONAL ? strdup("blue") : strdup("red");
		fprintf(file, "%s -> %s [color=%s]\n", link->src, link->dest, color);
	}
}