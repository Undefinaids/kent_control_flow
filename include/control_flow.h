//
// Created by antoine on 08/05/2020.
//

#ifndef KENT_CONTROL_FLOW_CONTROL_FLOW_H
#define KENT_CONTROL_FLOW_CONTROL_FLOW_H

typedef struct      block_s {
    //TODO add some fields
    struct block_s *next;
}                   block_t;

typedef struct      link_s {
    //TODO add some fields
    //ADD enum to know if it is a conditional link or not
    struct link_t *next;
}                   link_t;

// Manage global registers outside the blocks (allow to make a snapshot of them in order to save/load them)

typedef struct      program_s {
    const char **   instructions;
    unsigned int    counter;
    block_t *       blocks;
    link_t *        links;

}               program_t;
#endif //KENT_CONTROL_FLOW_CONTROL_FLOW_H
