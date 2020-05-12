//
// Created by antoine on 08/05/2020.
//

#include <control_flow.h>

char *epur_instruction(const char *instruction) {
    char *epur;
    size_t size_epured = 0;

    for (int i = 0; instruction[i] != '\0'; ++i)
        if (instruction[i] == ' ' && instruction[i + 1] == '(')
            for (; instruction[i] != '\0' && instruction[i] != ')'; ++i);
        else
            ++size_epured;
    if ((epur = calloc(sizeof(char), size_epured + 1)) == NULL) {
        ERROR("Failed to malloc clean instruction");
        return (NULL);
    }
    for (int i = 0, j = 0; instruction[i] != '\0'; ++i)
        if (instruction[i] == ' ' && instruction[i + 1] == '(')
            for (; instruction[i] != '\0' && instruction[i] != ')'; ++i);
        else {
            epur[j] = instruction[i];
            j++;
        }
    return (epur);
}

int parser(program_t *program) {
    for (; program->instructions[program->counter] != NULL; program->counter++) {
        if (strncmp(program->instructions[program->counter], "translation block start", 23) == 0) {
            LOG("New block\n");
            manage_new_block(program);
            //TODO Remove the return when we will handle multiple blocks
            return 0;
        }
    }
    return (0);
}

// TODO: Handle var and registers calculation in order to allow jump to dynamic address ? (handle/enable memory dumping)
