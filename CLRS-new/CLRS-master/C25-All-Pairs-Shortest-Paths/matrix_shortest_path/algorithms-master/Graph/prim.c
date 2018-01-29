/*
 * Prim is another algorithm for minimum spanning tree. Each step adds
 * to the tree A a light edge that connects A to an isolated vertex--
 * one on which no edge of A is incident.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "macros.h"
#include "minimum_spanning_tree.h"

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
extern void heapiup(void *base, size_t num, size_t width, size_t pos,\
					int64_t (*cmp)(const void *, const void *));
extern void chooseEdge(struct Graph *, int, int);

void initHeap(struct Heap *heap, struct Graph *, int);
static int64_t compare(const void *, const void *);
void decrease_key(struct Heap *, int, int, double);


double prim(struct Graph *graph)
{
	int source;
	double ans;
	struct HeapNode *minNode;
	struct Heap heap;
	struct Edge *edge;

	ans = 0;
	source = 0;
	initHeap(&heap, graph, source);
	heapSort(heap.heapNode, heap.heap_size, \
				sizeof(struct HeapNode), compare);

	while (heap.heap_size != 0) {
		minNode = (struct HeapNode *)extractTop(heap.heapNode, \
				&(heap.heap_size), sizeof(struct HeapNode), compare);
		if (minNode->to != source + 1) {
			chooseEdge(graph, minNode->from, minNode->to);
			ans += minNode->weight;
		}
		edge = graph->node[minNode->to].start;
		while (edge) {
			decrease_key(&heap, minNode->to, edge->to, edge->weight);
			edge = edge->next;
		}
	}
	return ans;
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

void decrease_key(struct Heap *heap, int from, int node, double weight)
{
	int i;
	int pos;

	pos = -1;
	assert(node > 0);
	for (i = 0; i < heap->heap_size; i++)
		if (heap->heapNode[i].to == node) {
			pos = i;
			break;
		}

	if (pos == -1 || heap->heapNode[pos].weight <= weight)
		return ;
	heap->heapNode[pos].weight = weight;
	heap->heapNode[pos].from = from;
	heapiup(heap->heapNode, heap->heap_size, \
			sizeof(struct HeapNode), pos, compare);
}
