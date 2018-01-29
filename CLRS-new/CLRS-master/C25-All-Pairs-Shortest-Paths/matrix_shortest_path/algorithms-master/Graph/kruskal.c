/*
 * Assume that we have a connected, undirected graph G = (V, E)
 * with a weight function w: E -> R, and we wish to find a 
 * minimum spanning tree for G. 
 * The greedy strategy is grows the minimum spanning tree one
 * edge at a time. At each step, we determine an edge (u, v)
 * that we can add to A that A + {(u, v)} is a subset of some
 * minimum spanning tree. We call such an edge a safe edge for A
 * There are two ways to determine a safe edge. Kruskal is one of 
 * them. Kruskal's algorithm finds a safe edge to add to the 
 * growing forest by finding of all the edges that connect any 
 * two trees in the forest, an edge (u, v) of least weight.
 */

#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "minimum_spanning_tree.h"

struct EdgeInfo { 
	int from;
	int to;
	double weight;
};

struct AllEdge {
	int edge_num;
	struct EdgeInfo edges[EDG_MAX];
};

struct Set {
	int prt[VER_MAX];
	int rank[VER_MAX];
};
	
extern void myqSort(void *base, size_t num, size_t width, \
					int64_t (*cmp)(const void *, const void *));
extern void chooseEdge(struct Graph *, int, int);
static void initAllEdge(struct AllEdge *);
static void findAllEdge(struct Graph *, struct AllEdge *);
static void makeSet(int, struct Set *);
static int findSet(int, struct Set *);
static void unionSet(int, int, struct Set *);
static void link(int, int, struct Set *);
static void initMakeSet(struct Graph *, struct Set *);
static int64_t compare(const void *, const void *);

double kruskal(struct Graph *graph)
{
	int i, j;
	int set1, set2;
	struct Set set;
	double ans;
	struct AllEdge allEdge;

	ans = 0;
	initAllEdge(&allEdge);
	findAllEdge(graph, &allEdge);
	initMakeSet(graph, &set);
	myqSort(allEdge.edges, allEdge.edge_num, sizeof(struct EdgeInfo), compare);

	for (i = 1, j = 0; i < graph->node_num && j < allEdge.edge_num; i++) {
		while ((set1 = findSet(allEdge.edges[j].from, &set)) == \
				(set2 = findSet(allEdge.edges[j].to, &set)))
			j++;
		chooseEdge(graph, allEdge.edges[j].from, allEdge.edges[j].to);
		ans += allEdge.edges[j].weight;
		unionSet(set1, set2, &set);
	}
	return ans;
}

void initAllEdge(struct AllEdge *allEdge)
{
	allEdge->edge_num = 0;
}

void findAllEdge(struct Graph *graph, struct AllEdge *allEdge)
{
	int i;
	struct Edge *edge; 
	struct EdgeInfo edgeInfo;

	for (i = 1; i <= graph->node_num; i++) {
		edge = graph->node[i].start;
		while (edge) {
			if (i < edge->to) {
				edgeInfo.from = i;
				edgeInfo.to = edge->to;
				edgeInfo.weight = edge->weight;
				allEdge->edges[allEdge->edge_num++] = edgeInfo;
			}
			edge = edge->next;
		}
	}
}

static int64_t compare(const void *elem1, const void *elem2)
{
	return ((struct EdgeInfo *)elem1)->weight - \
			((struct EdgeInfo *)elem2)->weight;
}

void makeSet(int pos, struct Set *set)
{
	set->prt[pos] = pos;
}

int findSet(int pos, struct Set *set)
{
	if (pos != set->prt[pos])
		set->prt[pos] = findSet(set->prt[pos], set);

	return set->prt[pos];
}

void unionSet(int pos1, int pos2, struct Set *set)
{
	link(findSet(pos1, set), findSet(pos2, set), set);
}

void link(int prt1, int prt2, struct Set *set)
{
	if (set->rank[prt1] < set->rank[prt2]) {
		set->prt[prt1] = prt2;
	} else {
		set->prt[prt2] = prt1;
		if (set->rank[prt1] == set->rank[prt2])
			set->rank[prt1]++;
	}
}

void initMakeSet(struct Graph *graph, struct Set *set)
{
	int i;

	for (i = 1; i <= graph->node_num; i++) {
		makeSet(i, set);
		set->rank[i] = 1;
	}
}
