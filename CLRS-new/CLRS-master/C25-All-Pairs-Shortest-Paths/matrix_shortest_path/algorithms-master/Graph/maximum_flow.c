#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "maximum_flow.h"

extern double ford_fulkerson(struct Graph *, \
				void (*findPath)(struct Graph *));

int main(void)
{
	int vertex, edge, src_no, dst_no;
	double maxFlow;
	struct Graph graph;

	while (scanf("%d %d", &vertex, &edge) != EOF) {
		src_no = 1;
		dst_no = vertex;
		initGraph(&graph, vertex, src_no, dst_no);
		input(&graph, edge);

		printf("use Ford-Fulkerson method:\n");
		maxFlow = ford_fulkerson(&graph, bfs);
		printf("the max flow is %lf\n", maxFlow);
	}

	return 0;
}

void initGraph(struct Graph *graph, int vertex, int src_no, int dst_no)
{
	int i, j;

	graph->ver_num = vertex;
	graph->src_no = src_no;
	graph->dst_no = dst_no;
	for (i = 0; i < vertex; i++) {
		for (j = 0; j < vertex; j++) {
			graph->capacity[i][j] = 0;
			graph->resiCapacity[i][j] = 0;
			graph->flow[i][j] = 0;
		}
		graph->precusor[i] = i;
		graph->visited[i] = 'n';
	}
}

void input(struct Graph *graph, int edge)
{
	int i;
	int from, to;
	double capacity;

	for (i = 0; i < edge; i++) {
		scanf("%d %d %lf", &from, &to, &capacity);
		graph->capacity[from][to] = capacity;
	}
}

void bfs(struct Graph *graph)
{
	int i, cur;
	struct Queue queue;

	queue.head = queue.tail = 0;

	if (graph->visited[graph->src_no] == 'n') {
		queue.queue[(queue.tail++)] = graph->src_no;
	}
	while (queue.head != queue.tail) {
		cur = queue.queue[(queue.head)++];
		graph->visited[cur] = 'y';
		if (cur == graph->dst_no)
			break;
		for (i = 1; i <= graph->ver_num; i++)
			if (graph->resiCapacity[cur][i] > 0 && \
						graph->visited[i] == 'n') {
				queue.queue[(queue.tail)++] = i;
				graph->precusor[i] = cur;
			}
	}
}
