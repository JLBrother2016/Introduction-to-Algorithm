/* 
 * A topological sort of a dag G = (V, E) is a linear ordering
 * of all its vertices such that if G contains an edge (u, v)
 * then u appears before v in the ordering.
 * Assume there exist topological sort of the graph
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph_traverse.h"

extern void visitNode(struct Graph *, int);
extern void dfsRecordTime(struct Graph *, int);
extern void myqSort(void *base, size_t num, size_t width, \
					int64_t (*cmp)(const void *, const void *));
extern void clearVisit(struct Graph *);

static int64_t compare(const void *elem1, const void *elem2);

void topos(struct Graph *graph)
{
	int i;

	assert(graph->node_num >= 1);
	for (i = 1; i <= graph->node_num; i++)
		if (graph->node[i].visited == 'n')
			dfsRecordTime(graph, i);
	clearVisit(graph);
	myqSort(&graph->node[1], graph->node_num, sizeof(struct Node), compare);
	for (i = 1; i <= graph->node_num; i++) {
		printf("start time:%d, end time:%d, ", \
			graph->node[i].start_time, graph->node[i].end_time);
		visitNode(graph, graph->node[i].from);
	}
}

static int64_t compare(const void *elem1, const void *elem2)
{
	return ((const struct Node *)elem2)->end_time - \
			((const struct Node *)elem1)->end_time;
}
