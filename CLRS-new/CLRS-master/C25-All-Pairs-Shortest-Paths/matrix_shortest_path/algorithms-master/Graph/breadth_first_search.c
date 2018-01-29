/*
 * Breadth-first search is one of the simplest algorithms for 
 * searching a graph. Given a graph G = (V, E) and a source
 * vertex s, BFS systematically explores the edges of G to 
 * discover every vertex that is reachable from already found
 * vertex set S and not have been visited. 
 */

#include <stdlib.h>
#include <assert.h>

#include "graph_traverse.h"

struct Queue {
	int queue[VER_MAX];
	int head;
	int tail;
};

extern void visitNode(struct Graph *, int);


void bfs(struct Graph *graph, int node)
{
	int i, cur;
	struct Queue queue;
	struct Edge *edge;

	assert(node >= 1 && node <= graph->node_num);

	queue.head = queue.tail = 0;

	if (graph->node[node].visited == 'n')
		queue.queue[(queue.tail)++] = node;
	while (queue.head != queue.tail) {
		cur = queue.queue[(queue.head)++];
		visitNode(graph, cur);
		graph->node[cur].visited = 'y';
		for (edge = graph->node[cur].start; edge != NULL; \
				edge = edge->next)
			if (graph->node[edge->to].visited == 'n')
				queue.queue[(queue.tail)++] = edge->to;
	}
}
