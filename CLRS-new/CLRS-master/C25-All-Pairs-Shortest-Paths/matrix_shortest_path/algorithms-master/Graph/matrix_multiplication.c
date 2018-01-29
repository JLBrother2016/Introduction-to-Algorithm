/*
 * This program use a dynamic-programming algorithm for the all-pairs
 * shortest paths problem on a directed graph G = (V, E).
 * As we know, all subpaths of a shortest path are shortest paths, and
 * the shortest path from a vertex to another vertex have at most |V-1|
 * Edges.
 * Using the idea of dynamic programming, we can consider the problem of
 * shortest path contains at most i edges each time.(0 <= i <= |V - 1|)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "macros.h"
#include "all_pairs_shortest_path.h"

void matrix_mul(double (*)[VER_MAX], double (*)[VER_MAX], \
							int (*)[VER_MAX], int (*)[VER_MAX], int);
void extend_edge(struct Matrix_Graph *);
void extend_edge_fast(struct Matrix_Graph *);

void matrix_mult(struct Matrix_Graph *mat_graph)
{
	int i, j;
/*
 * solution 1: every time extend an edge
 * time complexity n^4
 *
	for (i = 1; i < mat_graph->node_num; i++)
		extend_edge(mat_graph);
*/
/*
 * solution 2: extend 2^i edges respectively
 * time complexity n^3 * logn
 */
 	for (i = 1; i <= mat_graph->node_num; i++)
		for (j = 1; j <= mat_graph->node_num; j++) {
			mat_graph->tmp_matrix[i][j] = mat_graph->adjcent_matrix[i][j];
			mat_graph->tmp_precusor[i][j] = mat_graph->precusor[i][j];
		}
 	for (i = mat_graph->node_num - 1; j != 0; ) {
		extend_edge_fast(mat_graph);
		j /= 2;
	}
}

void extend_edge(struct Matrix_Graph *mat_graph)
{
	matrix_mul(mat_graph->shortest_matrix, mat_graph->adjcent_matrix, \
		mat_graph->precusor, mat_graph->precusor, mat_graph->node_num);	
}

void extend_edge_fast(struct Matrix_Graph *mat_graph)
{
	matrix_mul(mat_graph->tmp_matrix, mat_graph->tmp_matrix, \
	 mat_graph->tmp_precusor, mat_graph->tmp_precusor, mat_graph->node_num);
	matrix_mul(mat_graph->shortest_matrix, mat_graph->tmp_matrix, \
	 mat_graph->precusor, mat_graph->tmp_precusor, mat_graph->node_num);
}

void matrix_mul(double (*mat1)[VER_MAX], double (*mat2)[VER_MAX], \
		int (*precusor1)[VER_MAX], int (*precusor2)[VER_MAX], int size)
{
	int i, j, k;
	double dis1, dis2;
	double ans[VER_MAX][VER_MAX];

	for (i = 1; i <= size; i++)
		for (j = 1; j <= size; j++) {
			ans[i][j] = mat1[i][j];
			for (k = 1; k <= size; k++) {
				dis1 = mat1[i][k];
				dis2 = mat2[k][j];
				if (dis1 != INT_MAX && dis2 != INT_MAX && \
					ans[i][j] > dis1 + dis2) {
					ans[i][j] = dis1 + dis2;
					precusor1[i][j] = precusor2[k][j];
				}
			}
		}

	for (i = 1; i <= size; i++)
		for (j = 1; j <= size; j++)
			mat1[i][j] = ans[i][j];
}
