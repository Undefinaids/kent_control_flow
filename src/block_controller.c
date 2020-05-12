//
// Created by antoine on 12/05/2020.
//

#include "control_flow.h"

block_t *block_init(program_t *program) {
    block_t *block;

    if ((block = malloc(sizeof(block_t))) == NULL) {
        ERROR("Failed to malloc new block\n");
        return (NULL);
    }
    block->counter = 0;
    block->offset_counter = program->counter;
    block->labels = NULL;
    return (block);
}

label_t *label_push(label_t *label, char *name, unsigned int offset) {
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
    label_t *label;
    unsigned int count = 0;

    for (label = block->labels; label != NULL; label = label->next)
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

int block_get_labels(program_t *program, block_t *block) {
    // start at 1 in order to skip the first line
    for (unsigned int i = 1; program->instructions[program->counter + i] != NULL; ++i) {
        if (strncmp(program->instructions[program->counter + i], "translation block start", 23) == 0)
            return (labels_count(block));
        else if (strncmp(program->instructions[program->counter + i], "set label", 9) == 0) {
            //Think about trimming the name
            block->labels = label_push(block->labels, program->instructions[program->counter + i] + 9, i);
        }
    }
    return (labels_count(block));
}

int manage_new_block(program_t *program) {
    //TODO Retrieve the address of the block (instruction line)
    block_t *block = block_init(program);
    //Skip the first line (Translation block start)
    block->counter++;
    block_get_labels(program, block);
    label_debug(block);
    for (; program->instructions[program->counter + block->counter] != NULL; block->counter++) {
        if (strncmp(program->instructions[program->counter + block->counter], "translation block start", 23) == 0) {
            LOG("Exit block\n");
            program->counter += block->counter;
            return (0);
        }
        else {
            LOG("\t%s\n", program->instructions[program->counter + block->counter]);
        }
    }
    if (program->instructions[program->counter + block->counter] == NULL)
        LOG("Exit block\n");
    return (-1);
}