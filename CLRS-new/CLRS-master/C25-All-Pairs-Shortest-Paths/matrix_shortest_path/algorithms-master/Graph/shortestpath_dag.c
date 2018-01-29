/*
 * This is a implemention of single-source shortest path in
 * directed acyclic graphs. By relaxing the edges of a 
 * weighted dag G = (V, E) according to a topological sort 
 * of its vertices, we can compute shortest paths from a 
 * single source in O(V + E) time.
 * 
 * This algorithm can only used in directed acyclic graphs.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "single_shortest_path.h"

extern void myqSort(void *base, size_t num, size_t width, \
					int64_t (*cmp)(const void *, const void *));
extern int relax(struct Graph *, int, struct Edge *);

void dfsRecordTime(struct Graph *, int);
int64_t compare(const void *, const void *);

int time;

void shortestPathDAG(struct Graph *graph)
{
	int i;
	struct Edge *edge;

	time = 0;
	for (i = 1; i <= graph->node_num; i++)
		if (graph->node[i].visited == 'n')
			dfsRecordTime(graph, i);

	myqSort(&graph->node[1], graph->node_num, sizeof(struct Node), compare);
	for (i = 1; i <= graph->node_num; i++) {
		edge = graph->node[i].start;
		if (graph->node[i].distance == INT_MAX)
			continue;
		while (edge) {
			relax(graph, i, edge);
			edge = edge->next;
		}
	}
}

void dfsRecordTime(struct Graph * graph, int node)
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

int64_t compare(const void *elem1, const void *elem2)
{
	return ((struct Node *)elem2)->end_time - \
			((struct Node *)elem1)->end_time;
}
