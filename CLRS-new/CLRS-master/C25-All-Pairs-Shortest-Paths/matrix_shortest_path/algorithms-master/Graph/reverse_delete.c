/*
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "minimum_spanning_tree.h"

struct EdgeInfo { 
	int from;
	int to;
	double weight;
	int connectivity;
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
static void initAllEdge(struct AllEdge *allEdge);
static void findAllEdge(struct Graph *, struct AllEdge *);
static int64_t compare(const void *, const void *);
static void makeSet(int, struct Set *);
static int findSet(int, struct Set *);
static void unionSet(int, int, struct Set *);
static void link(int, int, struct Set *);
static void initMakeSet(struct Graph *, struct Set *);

double reverse_delete(struct Graph *graph)
{
	int i;
	int set1, set2;
	struct Set set;
	double ans;
	struct AllEdge allEdge;

	ans = 0;
	initAllEdge(&allEdge);
	findAllEdge(graph, &allEdge);
	initMakeSet(graph, &set);
	myqSort(allEdge.edges, allEdge.edge_num, sizeof(struct EdgeInfo), compare);

	for (i = 0; i < allEdge.edge_num; i++) {
		set1 = findSet(allEdge.edges[i].from, &set);
		set2 = findSet(allEdge.edges[i].to, &set);
		if (set1 == set2)
			allEdge.edges[i].connectivity = 1;
		else
			unionSet(set1, set2, &set);
	}

	for (i = allEdge.edge_num - 1; i >= 0; i--) {
		if (!allEdge.edges[i].connectivity) {
			chooseEdge(graph, allEdge.edges[i].from, allEdge.edges[i].to);
			ans += allEdge.edges[i].weight;
		} else {
			printf("delete edge from %d to %d, weight %2.1f\n", \
	allEdge.edges[i].from, allEdge.edges[i].to, allEdge.edges[i].weight);
		}
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
				edgeInfo.connectivity = 0;
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
	if (set->prt[pos] != pos)
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

