/*
 * Given a connected, undirected graph, a spanning tree of that graph
 * is a subgraph that is a tree and connects all the vertices together.
 * A minimum spanning tree(MST) is a spanning tree with weight less than
 * or equal to the weight of every spanning tree.
 * Input: the first line: numbers of vertexs N and numbers of edges E
 *		  then there are E lines, each line contain three numbers a,b,c
 *		  represent a edge weight c between a and b
 * Output: the weight of minimum spanning tree of this graph,
 *		   and corresponding choosen of the edges
 */

#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "minimum_spanning_tree.h"

extern double kruskal(struct Graph *);
extern double prim(struct Graph *);
extern double boruvka(struct Graph *);

int main(void)
{
	int node_num, edge_num;
	double ans;
	struct Graph graph;

	while (scanf("%d %d", &node_num, &edge_num) != EOF) {
		initGraph(&graph, node_num);
		input(&graph, edge_num);

		printf("Boruvka:\n");
		ans = boruvka(&graph);
		printf("the MST weigths %2.1f\n", ans);
		clearChoosedEdge(&graph);

		printf("kruskal:\n");
		ans = kruskal(&graph);
		printf("the MST weights %.2lf\n", ans);
		clearChoosedEdge(&graph);

		printf("prim:\n");
		ans = prim(&graph);
		printf("the MST weights %.2lf\n", ans);
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
		graph->node[i].start = NULL;
	}
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
		edge->choosed = 'n';
		graph->node[from].start = edge;

		edge = alloc(struct Edge, 1);
		edge->to = from;
		edge->weight = weight;
		edge->next = graph->node[to].start;
		edge->choosed = 'n';
		graph->node[to].start = edge;

	}
}

void chooseEdge(struct Graph *graph, int from, int to)
{
	struct Edge *edge;

	edge = graph->node[from].start;
	while (edge != NULL && edge->to != to)
		edge = edge->next;
	if (!edge)
		log(E, "can't find corresponding edge!");
	edge->choosed = 'y';
	printf("choose a safe edge from %d to %d, weight %.2lf\n", \
			from, to, edge->weight);
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

void clearChoosedEdge(struct Graph *graph)
{
	int i;
	struct Edge *edge;

	for (i = 1; i <= graph->node_num; i++) {
		edge = graph->node[i].start;
		while (edge) {
			edge->choosed = 'n';
			edge = edge->next;
		}
	}
}
