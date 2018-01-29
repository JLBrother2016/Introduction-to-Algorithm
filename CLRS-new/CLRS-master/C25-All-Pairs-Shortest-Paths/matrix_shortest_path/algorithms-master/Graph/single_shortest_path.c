/*
 *  In a shortest-paths problem, we are given a wighted, directed
 *  graph G = (V, E), with weight of all edges. We want to find
 *  a shortest path from a given source vertex s to each vertex 
 *  in V.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "macros.h"
#include "single_shortest_path.h"

#define SRC_NUM 1

extern int bellman_ford(struct Graph *);
extern void shortestPathDAG(struct Graph *);
extern void dijkstra(struct Graph *);

int main(void)
{
	int node_num, edge_num;
	int circuit;
	struct Graph graph;

	while (scanf("%d %d", &node_num, &edge_num) != EOF) {
		initGraph(&graph, node_num, SRC_NUM);
		input(&graph, edge_num);
		printf("The source node is %d\n", SRC_NUM);
		printf("bellman-ford:\n");
		circuit = bellman_ford(&graph);
		if (!circuit) {
			printf("this graph contains some negative weight cycle\n");
			continue;
		}
		output(&graph);
		clearChoose(&graph, SRC_NUM);

		printf("dijkstra:\n");
		dijkstra(&graph);
		output(&graph);

		printf("when the graph is dag:\n");
		shortestPathDAG(&graph);
		output(&graph);
		clearChoose(&graph, SRC_NUM);

		printf("\n");
		deleteEdge(&graph);
	}

}

void initGraph(struct Graph *graph, int node_num, int source)
{
	size_t i;

	assert(graph);
	graph->node_num = node_num;
	for (i = 1; i <= graph->node_num; i++)
		graph->node[i].start = NULL;
	clearChoose(graph, source);
}

void clearChoose(struct Graph *graph, int source)
{
	size_t i;

	assert(graph);
	for (i = 1; i <= graph->node_num; i++) {
		graph->node[i].from = i;
		graph->node[i].distance = INT_MAX;
		graph->node[i].precusor = -1;
		graph->node[i].visited = 'n';
	}
	graph->node[source].distance = 0;
	graph->node[source].precusor = source;
}

void input(struct Graph *graph, int edge_num)
{
	int i;
	int from, to;
	double weight;
	struct Edge *edge;

	for (i = 0; i < edge_num; i++) {
		scanf("%d %d %lf", &from, &to, &weight);
		edge = alloc(struct Edge, 1);
		edge->to = to;
		edge->weight = weight;
		edge->next = graph->node[from].start;
		graph->node[from].start = edge;
	}
}

void output(struct Graph *graph)
{
	size_t i;

	for (i = 1; i <= graph->node_num; i++)
		printf("node %d, distance %.2lf, precusor node %d\n", \
				graph->node[i].from, graph->node[i].distance, \
				graph->node[i].precusor);
}

int relax(struct Graph *graph, int from, struct Edge *edge)
{
	int to;
	double weight;

	to = edge->to;
	weight = edge->weight;
	if (graph->node[from].distance == INT_MAX || \
		graph->node[from].distance + weight >= graph->node[to].distance)
		return -1;

	graph->node[to].distance = graph->node[from].distance + weight;
	graph->node[to].precusor = from;
	return graph->node[to].distance;
}

void deleteEdge(struct Graph *graph)
{
	size_t i;
	struct Edge *tmp;

	for (i = 1; i <= graph->node_num; i++) {
		tmp = graph->node[i].start;
		while (tmp) {
			graph->node[i].start = tmp->next;
			free(tmp);
			tmp = graph->node[i].start;
		}
	}
}
	
