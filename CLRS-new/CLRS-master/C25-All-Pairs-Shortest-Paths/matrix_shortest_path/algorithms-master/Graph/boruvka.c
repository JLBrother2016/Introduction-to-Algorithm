/*
 * Boruvka's algorithm begins by first examining each vertex (component)
 * and adding the cheapest edge from that vertex (component) to another
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "macros.h"
#include "minimum_spanning_tree.h"

struct MinEdge {
	int from;
	int to;
	double weight;
};

struct Set {
	int component_num;
	int prt[VER_MAX];
	int rank[VER_MAX];
	struct MinEdge minEdge[VER_MAX];
};
	
extern void chooseEdge(struct Graph *, int, int);
static int findSet(int, struct Set *);
static void unionSet(int, int, struct Set *);
static void link(int, int, struct Set *);
static void initMakeSet(struct Graph *, struct Set *);
int count_component(struct Set *, int);

double boruvka(struct Graph *graph)
{
	int i, j;
	int set1, set2;
	int prev_component, curr_component;
	double ans;
	struct Edge *edge;
	struct Set set;

	ans = 0;
	prev_component = 1;
	initMakeSet(graph, &set);

	while ((curr_component = count_component(&set, graph->node_num)) \
					!= prev_component) {
		prev_component = curr_component;
		for (j = 1; j <= graph->node_num; j++)
			set.minEdge[j].weight = INT_MAX;

		for (i = 1; i <= graph->node_num; i++) {
			edge = graph->node[i].start;
			while (edge) {
				set1 = findSet(i, &set);
				set2 = findSet(edge->to, &set);
				if (set1 != set2) {
					if (edge->weight < set.minEdge[set1].weight) {
						set.minEdge[set1].weight = edge->weight;
						set.minEdge[set1].from = i;
						set.minEdge[set1].to = edge->to;
					}
					if (edge->weight < set.minEdge[set2].weight) {
						set.minEdge[set2].weight = edge->weight;
						set.minEdge[set2].from = edge->to;
						set.minEdge[set2].to = i;
					}
				}
				edge = edge->next;
			}
		}

		for (j = 1; j <= graph->node_num; j++) {
			if (set.minEdge[j].weight != INT_MAX) {
				chooseEdge(graph, set.minEdge[j].from, set.minEdge[j].to);
				if (findSet(set.minEdge[j].from, &set) != \
						findSet(set.minEdge[j].to, &set)) {
					unionSet(set.minEdge[j].from, set.minEdge[j].to, &set);
					ans += set.minEdge[j].weight;
				}
			}
		}
	}
	return ans;
}

static void makeSet(int pos, struct Set *set)
{
	set->prt[pos] = pos;
}

static int findSet(int pos, struct Set *set)
{
	if (pos != set->prt[pos])
		set->prt[pos] = findSet(set->prt[pos], set);

	return set->prt[pos];
}

static void unionSet(int pos1, int pos2, struct Set *set)
{
	link(findSet(pos1, set), findSet(pos2, set), set);
}

static void link(int prt1, int prt2, struct Set *set)
{
	if (set->rank[prt1] > set->rank[prt2]) {
		set->prt[prt2] = prt1; 
	} else {
		set->prt[prt1] = prt2;
		if (set->rank[prt1] == set->rank[prt2])
			set->rank[prt2]++;
	}
}

static void initMakeSet(struct Graph *graph, struct Set *set)
{
	int i;

	for (i = 1; i <= graph->node_num; i++) {
		makeSet(i, set);
		set->rank[i] = 1;
	}
}

int count_component(struct Set *set, int size)
{
	int i, ans;
	int count[VER_MAX];

	ans = 0;
	for (i = 1; i <= size; i++)
		count[i] = 0;
	for (i = 1; i <= size; i++)
		count[findSet(i, set)]++;
	for (i = 1; i <= size; i++)
		if (count[i])
			ans++;
	return ans;
}
