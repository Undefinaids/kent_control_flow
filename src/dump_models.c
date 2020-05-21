//
// Created by antoine on 20/05/2020.
//

#include "control_flow.h"
#include "dot.h"

static FILE *open_dot_file(const char *pathname) {
	FILE *file;

	if (pathname == NULL) {
		ERROR("Cannot create file, pathname is equal to NULL\n");
		return (NULL);
	}
	if ((file = fopen(pathname, "w+")) == NULL) {
		ERROR("Failed to create and open the output file\n");
		return (NULL);
	}
	return (file);
}

const char *get_dot_instructions(char **instructions, unsigned int offset, unsigned int size) {
	size_t nb_chars = 0;
	size_t i = 0;
	char *content;

	for (unsigned int j = 0; instructions[j + offset] != NULL && j < size; ++j, nb_chars += 2)
		nb_chars += strlen(instructions[j + offset]);
	if ((content = calloc(sizeof(char), nb_chars + 1)) == NULL) {
		ERROR("Failed to calloc condensed instructions\n");
		return (NULL);
	}
	for (unsigned int j = 0; instructions[j + offset] != NULL && j < size; ++j, i += 2) {
		for (unsigned int k = 0; instructions[j + offset][k] != '\0'; ++k) {
			content[i] = instructions[j + offset][k];
			++i;
		}
		content[i] = '\\';
		content[i + 1] = 'l';
	}
	return (content);
}

static void print_graph_header(FILE *file, mode_graph_t mode) {
	if (mode & O_STRICT)
		fprintf(file, "strict ");
	if (mode & O_DIRECT)
		fprintf(file, "di");
	fprintf(file, "graph graphname {\n");
}

static void print_graph_footer(FILE *file) {
	fprintf(file, "}\n");
}

void dump_models(program_t *program, const char *pathname, mode_graph_t mode) {
	FILE *file;
	dot_link_t *links = NULL;

	if ((file = open_dot_file(pathname)) == NULL)
		return;
	print_graph_header(file, mode);
	print_graph_blocks(file, program, &links);
	print_graph_links(file, links);
	print_graph_footer(file);
	fclose(file);
}