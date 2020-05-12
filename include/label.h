//
// Created by antoine on 12/05/2020.
//

#ifndef KENT_CONTROL_FLOW_LABEL_H
#define KENT_CONTROL_FLOW_LABEL_H

#include <stdlib.h>

typedef struct block_s block_t;

typedef struct      label_s {
    char *          name;
    unsigned int    offset;
    struct label_s *next;
}                   label_t;

void label_debug(block_t *block);
unsigned int labels_count(block_t *block);
label_t *label_push(label_t *label, const char *name, unsigned int offset);
unsigned int label_get_offset(block_t *block, const char *name);

#endif //KENT_CONTROL_FLOW_LABEL_H
