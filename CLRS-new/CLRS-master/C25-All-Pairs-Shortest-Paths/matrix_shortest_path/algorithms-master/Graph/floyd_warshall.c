/*
 * Floyd-Wallshall algorithm is another dynamic-programming 
 * algorithm to solve the all-pairs shortest path problem
 * on a directed graph G. 
 *
 * The Floyd-Wallshall algorithm considers the intermediate 
 * vertices of a shortest path. For any pair of vertices i, 
 * j, consider all paths from i to j whose intermediate
 * vertices are all drawn from {1, 2, ..., k}. The Floyd
 * algorithm exploits a relationship between path p and 
 * shortest paths from i to j with all intermediate vertices
 * in the set {1, 2, ..., k-1}.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "all_pairs_shortest_path.h"

void floyd_warshall(struct Matrix_Graph *graph)
{
	int i, j, k;
	double dis1, dis2;

	for (i = 1; i <= graph->node_num; i++)
		for (j = 1; j <= graph->node_num; j++) {
			graph->shortest_matrix[i][j] = graph->adjcent_matrix[i][j];
			graph->precusor[i][j] = i;
		}

	for (k = 1; k <= graph->node_num; k++)
		for (i = 1; i <= graph->node_num; i++)
			for (j = 1; j <= graph->node_num; j++) {
				dis1 = graph->shortest_matrix[i][k];
				dis2 = graph->shortest_matrix[k][j];
				if (dis1 != INT_MAX && dis2 != INT_MAX && \
					dis1 + dis2 < graph->shortest_matrix[i][j]) {
					graph->shortest_matrix[i][j] = dis1 + dis2;
					if (k != j)
						graph->precusor[i][j] = graph->precusor[k][j];
				}
			}
					
}
