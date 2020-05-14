#include <stdio.h>
#include "dot_graph.h"
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
    if (program->instructions == NULL)
        return (NULL);
    program->counter = 0;
    //Direct graph (option will be add later);
    program->graph = dotgraph_init(0);
    if (program->graph == NULL)
    	return (NULL);
    return (program);
}

int main(int argc, char **argv) {
    program_t  *program;

    if (argc != 2)
        return (-1);
    if ((program = init_program(argv[1])) == NULL) {
        return (-1);
    }
    parser(program);
    dump_dot(program, "test.dot");
    return (0);
}
