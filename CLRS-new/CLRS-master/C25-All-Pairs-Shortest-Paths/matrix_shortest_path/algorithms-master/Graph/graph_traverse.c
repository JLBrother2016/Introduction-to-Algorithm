/*
 * Input: the first line : numbers of vertexs N and numbers of edges E
 * 		  then there are E lines, each line contain two number a, b
 * 		  represent an edge from a to b.
 * Output: Assume vertex 1 is the traverse source node, traverse all
 *		   the node in the graph.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph_traverse.h"
#include "macros.h"

extern void dfs(struct Graph *graph, int node, \
				void (*visitNode)(struct Graph *, int));
extern void bfs(struct Graph *graph, int node);
extern void topos(struct Graph *graph);

int time;

int main(void)
{
	int node_num, edge_num, source;
	struct Graph graph;

	time = 0;
	source = 1;
	while (scanf("%d %d", &node_num, &edge_num) != EOF) {
		initGraph(&graph, node_num);
		input(&graph, edge_num);

		printf("source node of dfs and bfs is %d:\n", source);
		printf("depth first search:\n");
		for (source = 1; source <= graph.node_num; source++)
			dfs(&graph, source, visitNode);
		clearVisit(&graph);	

		printf("breadth first search:\n");
		for (source = 1; source <= graph.node_num; source++)
			bfs(&graph, source);
		clearVisit(&graph);

		printf("traverse by topological order:\n");
		topos(&graph);

		deleteEdge(&graph);
	}

	return 0;
}

void initGraph(struct Graph *graph, int node_num)
{
	int i;

	graph->node_num = node_num;
	for (i = 1; i <= graph->node_num; i++) {
		graph->node[i].from = i;
		graph->node[i].visited = 'n';
		graph->node[i].start = NULL;
	}
}

void input(struct Graph *graph, int edge_num)
{
	int i;
	int from, to;
	struct Edge *edge;
	
	for (i = 0; i < edge_num; i++) {
		scanf("%d %d", &from, &to);
		edge = alloc(struct Edge, 1);
		edge->to = to;
		edge->next = graph->node[from].start;
		graph->node[from].start = edge;
	}
}

void visitNode(struct Graph *graph, int node)
{
	assert(node >= 1 && node <= graph->node_num);
	if (graph->node[node].visited == 'n')
		printf("visit node %d\n", node);
}

void clearVisit(struct Graph *graph)
{
	int i;
		
	assert(graph->node_num >= 1);
	for (i = 1; i <= graph->node_num; i++)
		graph->node[i].visited = 'n';
}

void deleteEdge(struct Graph *graph)
{
	int i;
	struct Edge *edge;

	for (i = 1; i <= graph->node_num; i++) {
		edge = graph->node[i].start;
		while (edge) {
			graph->node[i].start = edge->next;
			free(edge);
			edge = graph->node[i].start;
		}
	}
}
