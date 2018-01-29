/*
 * Dijkstra is another algorithm to solve the single shortest
 * path problem for the case in which all edge weights are
 * nonnegative.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "macros.h"
#include "single_shortest_path.h"

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

void initHeap(struct Heap *, struct Graph *, int);
static int64_t compare(const void *, const void *);
void decrease_key(struct Graph *, struct Heap *, int, struct Edge *);

void dijkstra(struct Graph *graph)
{
	int source;
	struct HeapNode *minNode;
	struct Heap heap;
	struct Edge *edge;

	source = 0;
	initHeap(&heap, graph, source);
	heapSort(heap.heapNode, heap.heap_size, \
				sizeof(struct HeapNode), compare);

	while (heap.heap_size != 0) {
		minNode = (struct HeapNode *)extractTop(heap.heapNode, \
				&(heap.heap_size), sizeof(struct HeapNode), compare);
		printf("choose vertex %d, distance %.2lf\n",\
				minNode->to, minNode->weight);
		edge = graph->node[minNode->to].start;
		while (edge) {
			decrease_key(graph, &heap, minNode->to, edge);
			edge = edge->next;
		}
	}
}

void initHeap(struct Heap *heap, struct Graph *graph, int source)
{
	int i;

	heap->heap_size = graph->node_num;
	for (i = 0; i < graph->node_num; i++) {
		heap->heapNode[i].weight = INT_MAX;
		heap->heapNode[i].to = i + 1;
	}
	heap->heapNode[source].weight = 0;
}

static int64_t compare(const void *elem1, const void *elem2)
{
	return ((struct HeapNode *)elem2)->weight - \
			((struct HeapNode *)elem1)->weight;
}

void decrease_key(struct Graph *graph, struct Heap *heap, \
					int from, struct Edge *edge)
{
	int i;
	int pos, res;

	pos = -1;
	assert(from > 0 && from <= graph->node_num);
	for (i = 0; i < heap->heap_size; i++)
		if (heap->heapNode[i].to == edge->to) {
			pos = i;
			break;
		}

	if (pos == -1)
		return ;
	if ((res = relax(graph, from, edge)) == -1)
		return ;
	heap->heapNode[pos].weight = res;
	heap->heapNode[pos].from = from;
	heapiup(heap->heapNode, heap->heap_size, \
			sizeof(struct HeapNode), pos, compare);
}
