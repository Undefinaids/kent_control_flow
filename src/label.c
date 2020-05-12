//
// Created by antoine on 12/05/2020.
//

#include "control_flow.h"
#include "error.h"
#include "label.h"

label_t *label_push(label_t *label, const char *name, unsigned int offset) {
    label_t *new_label;
    label_t *iterator;

    if ((new_label = malloc(sizeof(label_t))) == NULL) {
        ERROR("Failed to malloc new label\n");
        return (NULL);
    }
    new_label->offset = offset;
    new_label->name = strdup(name);
    new_label->next = NULL;
    if (label == NULL) {
        return (new_label);
    }
    for (iterator = label; iterator->next != NULL; iterator = iterator->next);
    iterator->next = new_label;
    return (label);
}

unsigned int labels_count(block_t *block) {
    unsigned int count = 0;

    for (label_t *label = block->labels; label != NULL; label = label->next)
        ++count;
    return (count);
}

void label_debug(block_t *block) {
    label_t *label;

    LOG("Label debug beginning\n");
    for (label = block->labels; label != NULL; label = label->next)
        LOG("- Label '%s' set at offset %d\n", label->name, label->offset);
    LOG("Label debug end\n");
}

unsigned int label_get_offset(block_t *block, const char *name) {
    for (label_t *label = block->labels; label != NULL; label = label->next) {
        if (strcmp(label->name, name) == 0)
            return (label->offset);
    }
}