/*
 * The Bellman-Ford algorithm solves the single-source shortest-paths
 * problem in the general case in which edge weights may be negative.
 * Given a weighted, directed graph G = (V, E) with source s and 
 * weight function w: E->R, the Bellman-Ford algorithm returns a 
 * boolean value indicating whether or not there is a negative-weight
 * cycle that is reachable from the source. If there is such a cycle,
 * the algorithm indicates that no solution exists. If there is no
 * such cycle, the algorithm produces the shortest paths and their 
 * weights.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "single_shortest_path.h"

extern int relax(struct Graph *, int, struct Edge *);

int bellman_ford(struct Graph *graph)
{
	int i, j;
	struct Edge *edge;

	for (i = 1; i < graph->node_num; i++)
		for (j = 1; j <= graph->node_num; j++) {
			edge = graph->node[j].start;
			if (graph->node[j].distance == INT_MAX)
				continue;
			while (edge) {
				relax(graph, j, edge);
				edge = edge->next;
			}
		}

	for (j = 1; j <= graph->node_num; j++) {
		edge = graph->node[j].start;
		while (edge) {
			if (relax(graph, j, edge) != -1)
				return 0;
			edge = edge->next;
		}
	}
	return 1;
}
