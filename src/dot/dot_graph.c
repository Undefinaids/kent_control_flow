//
// Created by antoine on 13/05/2020.
//

#include "dot_graph.h"

dotgraph_t *dotgraph_init(mode_graph_t mode) {
	dotgraph_t *graph;

	graph = malloc(sizeof(dotgraph_t));
	if (graph == NULL) {
		return (NULL);
	}
	graph->strict = mode & O_STRICT ? true : false;
	graph->undirect = mode & O_UNDIRECT ? true : false;
	graph->blocks = NULL;
	graph->links = NULL;
	return (graph);
}