/* 
 * A topological sort of a dag G = (V, E) is a linear ordering
 * of all its vertices such that if G contains an edge (u, v)
 * then u appears before v in the ordering.
 * Assume there exist topological sort of the graph
 * Input: the number of vertexs N and the number of edges E
 *		  then E lines, each line has two numbers a, b,
 * 		  representing there is a edge from a to b
 * Output: output one of the topoligical sort of the graph
 */

#include <stdio.h>
#include <stdlib.h>

#define VER_MAX 100

struct Edge {
	int to;
	struct Edge *next;
};

struct Node {
	int from;
	char visited;
	int start_time;
	int end_time;
	struct Edge *start;
};

struct Graph {
	int node_num;
	struct Node node[VER_MAX];
};

void input(struct Graph *, int, int);
void dfs(struct Graph *, int *, int);
void output(struct Graph *);
void quickSort(struct Graph *, int, int);
int partition(struct Graph *, int, int);
void copyNode(struct Node *, struct Node *);

int main(void)
{
	int node_num, edge_num;
	int i, time;
	struct Graph graph;

	time = 0;
	while (scanf("%d %d", &node_num, &edge_num) != EOF) {
		input(&graph, node_num, edge_num);
		for (i = 1; i <= graph.node_num; i++)
			if (graph.node[i].visited == 'n')
				dfs(&graph, &time, 1);
		output(&graph);
	}

	return 0;
}

void input(struct Graph *graph, int node_num, int edge_num)
{
	int i;
	int from, to;
	struct Edge *edge;

	graph->node_num = node_num;
	for (i = 1; i <= node_num; i++) {
		graph->node[i].from = i;
		graph->node[i].visited = 'n';
		graph->node[i].start = NULL;
	}
	for (i = 0; i < edge_num; i++) {
		scanf("%d %d", &from, &to);
		if (!(edge = (struct Edge *)malloc(sizeof(struct Edge))))
			exit(1);
		edge->to = to;
		edge->next = graph->node[from].start;
		graph->node[from].start = edge;
	}
}

void dfs(struct Graph *graph, int *time, int node)
{
	int i;
	struct Edge *edge;

	if (node > graph->node_num || graph->node[node].visited == 'y')
		return ;

	graph->node[node].start_time = (*time)++;
	graph->node[node].visited = 'y';
	for (edge = graph->node[node].start; edge != NULL; edge = edge->next)
		dfs(graph, time, edge->to);
	graph->node[node].end_time = (*time)++;
}

void output(struct Graph *graph)
{
	int i;

	printf("One of topological sort of the graph:\n");
	quickSort(graph, 1, graph->node_num);
	for (i = 1; i <= graph->node_num; i++)
		printf("%d %d-%d\n", graph->node[i].from, \
			graph->node[i].start_time, graph->node[i].end_time);
	printf("\n");
}

void quickSort(struct Graph *graph, int low, int high)
{
	int pos;

	if (low >= high)
		return ;
	pos = partition(graph, low, high);
	quickSort(graph, low, pos - 1);
	quickSort(graph, pos + 1, high);
}

int partition(struct Graph *graph, int low, int high)
{
	int i, j;
	struct Node tmp;

	i = low;
	j = high;
	copyNode(&graph->node[low], &tmp);
	while (i < j) {
		while (i < j && graph->node[j].end_time <= tmp.end_time)
			j--;
		copyNode(&graph->node[j], &graph->node[i]);
		while (i < j && graph->node[i].end_time >= tmp.end_time)
			i++;
		copyNode(&graph->node[i], &graph->node[j]);
	}
	copyNode(&tmp, &graph->node[i]);
	return i;
}

void copyNode(struct Node *src, struct Node *dst)
{
	dst->from = src->from;
	dst->visited = src->visited;
	dst->start_time = src->start_time;
	dst->end_time = src->end_time;
	dst->start = src->start;
}
