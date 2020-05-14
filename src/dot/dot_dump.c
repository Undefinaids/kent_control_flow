//
// Created by antoine on 08/05/2020.
//

//Function made to dump all the dot blocks and links to a dot file

#include "control_flow.h"

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

static void print_graph_header(FILE *file, dotgraph_t *graph) {
	if (graph->strict)
		fprintf(file, "strict ");
	if (graph->undirect == false)
		fprintf(file, "di");
	fprintf(file, "graph graphname {\n");
}

static void close_graph(FILE *file) {
	fprintf(file, "}\n");
	fclose(file);
}

void dump_dot(program_t *program, const char *pathname) {
	FILE *file;

	if ((file = open_dot_file(pathname)) == NULL)
		return;
	print_graph_header(file, program->graph);
	// Dump Blocks and Links here
	close_graph(file);
}