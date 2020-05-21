//
// Created by antoine on 19/05/2020.
//

#include "control_flow.h"

label_t *init_label(const char *name, unsigned int offset) {
	label_t *label;

	if ((label = malloc(sizeof(label_t))) == NULL) {
		ERROR("Failed to malloc a new block_t");
		return (NULL);
	}
	label->name = strdup(name);
	label->offset = offset;
	label->next = NULL;
	return (label);
}

label_t *push_label(label_t **labels, const char *name, unsigned int offset) {
	label_t *label;
	label_t *iterator;

	if ((label = init_label(name, offset)) == NULL) {
		return (NULL);
	}
	if (*labels == NULL) {
		*labels = label;
		return (label);
	}
	for (iterator = *labels; iterator->next != NULL; iterator = iterator->next);
	iterator->next = label;
	return (label);
}

unsigned int get_offset_by_label_name(label_t *label, const char *name) {
	for (label_t *iterator = label; iterator != NULL; iterator = iterator->next) {
		if (strcmp(name, iterator->name) == 0)
			return (iterator->offset);
	}
	return (0);
}