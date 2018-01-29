/*
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "macros.h"
#include "all_pairs_shortest_path.h"

extern void matrix_mult(struct Matrix_Graph *graph);
extern void floyd_warshall(struct Matrix_Graph *graph);
extern void johnson(struct Matrix_Graph *, struct Point_Graph *);

int main(void)
{
	int node_num, edge_num;
	struct Matrix_Graph mat_graph;
	struct Point_Graph poi_graph;

	while (scanf("%d %d", &node_num, &edge_num) != EOF) {
		initGraph(&mat_graph, &poi_graph, node_num);
		input(&mat_graph, &poi_graph, edge_num);
		printf("matrix multiplication:\n");
		matrix_mult(&mat_graph);
		output(&mat_graph);
		clearChoose(&mat_graph, &poi_graph);

		printf("floyd warshall:\n");
		floyd_warshall(&mat_graph);
		output(&mat_graph);
		clearChoose(&mat_graph, &poi_graph);

		printf("johnson:\n");
		johnson(&mat_graph, &poi_graph);
		output(&mat_graph);
		
		clearEdge(&poi_graph);
	}
	
	return 0;
}

void initGraph(struct Matrix_Graph *mat_graph, \
				struct Point_Graph *poi_graph, int node_num)
{
	int i, j;

	mat_graph->node_num = node_num;
	poi_graph->node_num = node_num;

	for (i = 1; i <= mat_graph->node_num; i++) {
		for (j = 1; j <= mat_graph->node_num; j++) {
			mat_graph->adjcent_matrix[i][j] = INT_MAX;
			mat_graph->shortest_matrix[i][j] = INT_MAX;
			mat_graph->precusor[i][j] = -1;
		}
		mat_graph->adjcent_matrix[i][i] = 0;
		mat_graph->shortest_matrix[i][i] = 0;
		mat_graph->precusor[i][i] = i;
	}

	for (i = 1; i <= poi_graph->node_num; i++) {
		poi_graph->node[i].start = NULL;
		poi_graph->node[i].distance = INT_MAX;
		poi_graph->node[i].from = i;
	}
}

void clearChoose(struct Matrix_Graph *mat_graph, \
				struct Point_Graph *poi_graph)
{
	int i, j;

	for (i = 1; i <= mat_graph->node_num; i++) {
		for (j = 1; j <= mat_graph->node_num; j++) {
			mat_graph->precusor[i][j] = -1;
			mat_graph->shortest_matrix[i][j] = INT_MAX;
		}
		mat_graph->precusor[i][i] = i;
		mat_graph->shortest_matrix[i][i] = 0;
	}

	for (i = 1; i <= poi_graph->node_num; i++)
		poi_graph->node[i].distance = INT_MAX;
}

void input(struct Matrix_Graph *mat_graph, \
			struct Point_Graph *poi_graph, int edge_num)
{
	int i;
	int from, to;
	double weight;
	struct Edge *edge;

	for (i = 0; i < edge_num; i++) {
		scanf("%d %d %lf", &from, &to, &weight);
		mat_graph->adjcent_matrix[from][to] = weight;

		edge = alloc(struct Edge, 1);
		edge->to = to;
		edge->weight = weight;
		edge->newWeight = weight;
		edge->next = poi_graph->node[from].start;
		poi_graph->node[from].start = edge;
		mat_graph->precusor[from][to] = from;
	}
}

void output(struct Matrix_Graph *mat_graph)
{
	int i, j;

	printf("shortest distance matrix:\n");
	for (i = 1; i <= mat_graph->node_num; i++) {
		for (j = 1; j <= mat_graph->node_num; j++)
			printf("%8.2lf", mat_graph->shortest_matrix[i][j]);
		printf("\n");
	}	
	printf("correspond precusor:\n");
	for (i = 1; i <= mat_graph->node_num; i++) {
		for (j = 1; j <= mat_graph->node_num; j++)
			printf("%8d", mat_graph->precusor[i][j]);
		printf("\n");
	}	
}

void clearEdge(struct Point_Graph *poi_graph)
{
	int i;
	struct Edge *edge;

	for (i = 1; i <=poi_graph->node_num; i++) {
		edge = poi_graph->node[i].start;
		while (edge) {
			poi_graph->node[i].start = edge->next;
			free(edge);
			edge = poi_graph->node[i].start;
		}
	}
}
