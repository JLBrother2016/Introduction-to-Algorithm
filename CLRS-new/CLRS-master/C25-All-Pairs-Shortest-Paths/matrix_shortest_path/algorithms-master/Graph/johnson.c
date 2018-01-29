/*
 * Johnson's algorithm uses the technique of reweighting.
 * If all edge weights w in a graph G are nonnegative, we
 * can find shortest paths between all pairs of vertices by
 * running Dijkstra's algorithm once from each vertex.
 * The new set of edge weights w' must satisfy two important
 * properties:
 * 1. a path p is a shortest path from u to v using weight
 * function w iff p is also a shortest path from u to v using
 * weight function w'.
 * 2. for all edges, the new weight is nonnegative.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "macros.h"
#include "all_pairs_shortest_path.h"

struct HeapNode {
	double weight;
	int from;
	int to;
};

struct Heap {
	size_t heap_size;
	struct HeapNode heapNode[VER_MAX];
};

extern void heapSort(void *base, size_t num, size_t width, \
					int64_t (*cmp)(const void *, const void *));
extern void *extractTop(void *base, size_t *num, size_t width, \
					int64_t (*cmp)(const void *, const void *));
extern void heapiup(void *base, size_t num, size_t width, size_t pos, \
					int64_t (*cmp)(const void *, const void *));

int reweighttingEdge(struct Matrix_Graph *, struct Point_Graph *);
void reweighttingPath(struct Matrix_Graph *);
int relax(struct Matrix_Graph *, int, int, struct Edge *);
int bellman_ford(struct Matrix_Graph *, struct Point_Graph *, int);
void dijkstra(struct Matrix_Graph *, struct Point_Graph *, int);
void initHeap(struct Heap *, struct Point_Graph *, int);
static int64_t compare(const void *, const void *);
void decrease_key(struct Matrix_Graph *, struct Heap *, int, int, struct Edge *);

void johnson(struct Matrix_Graph *mat_graph, struct Point_Graph *poi_graph)
{
	int i;
	
	if(!reweighttingEdge(mat_graph, poi_graph))
		log(E, "the graph contain some negative circult");
	for (i = 1; i <= poi_graph->node_num; i++)
		dijkstra(mat_graph, poi_graph, i);
	reweighttingPath(mat_graph);
}

int reweighttingEdge(struct Matrix_Graph *mat_graph, \
						struct Point_Graph *poi_graph)
{
	int i, j;
	int newNode, noNegativeCircult;
	struct Edge *edge;

	newNode = 0;
	poi_graph->node[newNode].start = NULL;
	poi_graph->node[newNode].from = newNode;
	mat_graph->adjcent_matrix[newNode][newNode] = 0;
	for (i = 1; i <= poi_graph->node_num; i++) {
		edge = alloc(struct Edge, 1);
		edge->to = i;
		edge->weight = 0;
		edge->next = poi_graph->node[newNode].start;
		poi_graph->node[newNode].start = edge;
		mat_graph->adjcent_matrix[newNode][i] = INT_MAX;
	}
	for (j = 0; j <= mat_graph->node_num; j++)
		mat_graph->shortest_matrix[0][j] = mat_graph->adjcent_matrix[0][j];

	if (!(noNegativeCircult = bellman_ford(mat_graph, poi_graph, newNode)))
		return noNegativeCircult;

	for (i = 1; i <= poi_graph->node_num; i++) {
		edge = poi_graph->node[i].start;
		while (edge) {
			edge->newWeight = edge->weight + \
							mat_graph->shortest_matrix[newNode][i] \
							- mat_graph->shortest_matrix[newNode][edge->to];
			edge = edge->next;
		}
	}
		
	return noNegativeCircult;
}

void reweighttingPath(struct Matrix_Graph *mat_graph)
{
	int i, j;

	for (i = 1; i <= mat_graph->node_num; i++)
		for (j = 1; j <= mat_graph->node_num; j++)
			mat_graph->shortest_matrix[i][j] += \
				(mat_graph->shortest_matrix[0][j] - \
				mat_graph->shortest_matrix[0][i]);
}

int bellman_ford(struct Matrix_Graph *mat_graph, \
				struct Point_Graph *poi_graph, int source)
{
	int i, j;
	struct Edge *edge;

	for (i = 0; i < poi_graph->node_num; i++)
		for (j = 0; j < poi_graph->node_num; j++) {
			edge = poi_graph->node[j].start;
			if (mat_graph->shortest_matrix[source][j] == INT_MAX)
				continue;
			while (edge) {
				relax(mat_graph, source, j, edge);
				edge = edge->next;
			}
		}

	for (j = 0; j <= poi_graph->node_num; j++) {
		edge = poi_graph->node[j].start;
		while (edge) {
			if (relax(mat_graph, source, j, edge) != -1)
				return 0;
			edge = edge->next;
		}
	}
	return 1;
}

int relax(struct Matrix_Graph *mat_graph, int source, \
						int from, struct Edge *edge)
{
	int to;
	double weight;

	to = edge->to;
	weight = edge->newWeight;
	if (mat_graph->shortest_matrix[source][from] == INT_MAX || \
		mat_graph->shortest_matrix[source][from] + weight >= \
		mat_graph->shortest_matrix[source][to])
		return -1;

	mat_graph->shortest_matrix[source][to] = \
		mat_graph->shortest_matrix[source][from] + weight;
	mat_graph->precusor[source][to] = from;
	return mat_graph->shortest_matrix[source][to];
}

void dijkstra(struct Matrix_Graph *mat_graph, struct Point_Graph *poi_graph, \
				int source)
{
	struct HeapNode *minNode;
	struct Heap heap;
	struct Edge *edge;

	initHeap(&heap, poi_graph, source);
	heapSort(heap.heapNode, heap.heap_size, \
			sizeof(struct HeapNode), compare);

	while (heap.heap_size != 0) {
		minNode = (struct HeapNode *)extractTop(heap.heapNode, \
			&(heap.heap_size), sizeof(struct HeapNode), compare);
		edge = poi_graph->node[minNode->to].start;
		while (edge) {
			decrease_key(mat_graph, &heap, source, minNode->to, edge);
			edge = edge->next;
		}
	}
}

void initHeap(struct Heap *heap, struct Point_Graph *graph, int source)
{
	int i;

	heap->heap_size = graph->node_num;
	for (i = 0; i < graph->node_num; i++) {
		heap->heapNode[i].weight = INT_MAX;
		heap->heapNode[i].to = i + 1;
		heap->heapNode[i].from = -1;
	}
	heap->heapNode[source - 1].weight = 0;
	heap->heapNode[source - 1].from = source;
}

static int64_t compare(const void *elem1, const void *elem2)
{
	return ((struct HeapNode *)elem2)->weight - \
			((struct HeapNode *)elem1)->weight;
}

void decrease_key(struct Matrix_Graph *mat_graph, struct Heap *heap, \
					int source, int from, struct Edge *edge)
{
	int i;
	int pos, res;

	pos = -1;
	assert(from > 0 && from <= mat_graph->node_num);
	for (i = 0; i < heap->heap_size; i++)
		if (heap->heapNode[i].to == edge->to) {
			pos = i;
			break;
		}

	if (pos == -1)
		return ;
	if ((res = relax(mat_graph, source, from, edge)) == -1)
		return ;
	heap->heapNode[pos].weight = res;
	heap->heapNode[pos].from = from;
	heapiup(heap->heapNode, heap->heap_size, \
			sizeof(struct HeapNode), pos, compare);
}
