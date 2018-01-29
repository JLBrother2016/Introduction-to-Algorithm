/*
 * Depth-first search is another simplest algorithms for 
 * searching a graph. Given a graph G = (V, E) and a source
 * vertex s, DFS explores edges out of the most recently 
 * discovered vertex v that still has unexplored edges
 * leaving it.
 */

#include <stdlib.h>
#include <assert.h>

#include "graph_traverse.h"

extern int time;
//extern void visitNode(struct Graph *, int);

void dfsRecordTime(struct Graph *graph, int node)
{
	struct Edge *edge;

	assert(node <= graph->node_num && node >= 1);
	graph->node[node].visited = 'y';
	graph->node[node].start_time = time++;
	for (edge = graph->node[node].start; edge != NULL; edge = edge->next)
		if (graph->node[edge->to].visited == 'n')
			dfsRecordTime(graph, edge->to);
	graph->node[node].end_time = time++;
}

void dfs(struct Graph *graph, int node, \
		void (*visitNode)(struct Graph *, int))
{
	struct Edge *edge;
	
	assert(node <= graph->node_num && node >= 1);
	visitNode(graph, node);
	graph->node[node].visited = 'y';
	for (edge = graph->node[node].start; edge != NULL; edge = edge->next)
		if (graph->node[edge->to].visited == 'n')
			dfs(graph, edge->to, visitNode);
}
