#include <stdio.h>
#include "control_flow.h"
#include "file.h"

void debug_text(const char **text) {
    LOG("################\n");
    for (int i = 0; text[i] != NULL; ++i)
        LOG("#%s#\n", text[i]);
    LOG("################\n");
}

program_t *init_program(const char *pathname) {
    program_t *program;

    if ((program = malloc(sizeof(program_t))) == NULL) {
        ERROR("Failed to malloc program_t struct");
        return (NULL);
    }
    program->instructions = read_and_format_file(pathname);
    program->counter = 0;
    return (program);
}

int main(int argc, char **argv) {
    if (argc != 2)
        return (-1);
    init_program(argv[1]);
    return 0;
}
