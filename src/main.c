#include "control_flow.h"
#include "file.h"
#include "dot.h"

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
	dump_models(program, "output.dot", O_DIRECT | O_STRICT);
	return (0);
}
