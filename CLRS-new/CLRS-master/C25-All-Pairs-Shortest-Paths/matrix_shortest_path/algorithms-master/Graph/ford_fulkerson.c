#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "macros.h"
#include "maximum_flow.h"

typedef double (*FindPath)(struct Graph *graph);

void createResidualNet(struct Graph *);
double findAugmentPath(struct Graph *, FindPath);
//double bfs(struct Graph *);

double ford_fulkerson(struct Graph *graph, FindPath findPath)
{
	double maxFlow, newFlow;
	
	maxFlow = 0;
	createResidualNet(graph);
	while ((newFlow = findAugmentPath(graph, findPath)) > 0)
		maxFlow += newFlow;

	return maxFlow;
}

void createResidualNet(struct Graph *graph)
{
	int i, j;

	for (i = 1; i <= graph->ver_num; i++)
		for (j = 1; j <= graph->ver_num; j++) {
			if (graph->capacity[i][j]) {
				if (graph->capacity[j][i])
					log(E, "can't have u->v and v->u exist!");
				graph->resiCapacity[i][j] = graph->capacity[i][j] - \
											graph->flow[i][j];
				graph->resiCapacity[j][i] = graph->flow[i][j];
			}
		}
}

double findAugmentPath(struct Graph *graph, FindPath findPath)
{
	int i, cur, pre;
	double addFlow;

	for (i = 1; i <= graph->ver_num; i++)
		graph->visited[i] = 'n';

	findPath(graph);
	if (graph->visited[graph->dst_no] == 'y') {
		cur = graph->dst_no;
		addFlow = INT_MAX;
		while (cur != graph->src_no) {
			pre = graph->precusor[cur];
			if (addFlow > graph->resiCapacity[pre][cur])
				addFlow = graph->resiCapacity[pre][cur];
			cur = pre;
		}

		cur = graph->dst_no;
		while (cur != graph->src_no) {
			pre = graph->precusor[cur];
			graph->resiCapacity[pre][cur] -= addFlow;
			graph->resiCapacity[cur][pre] += addFlow;
			graph->flow[pre][cur] += addFlow;
			printf("flow add on path %d to %d, with %lf\n", \
										pre, cur, addFlow);
			cur = pre;
		}
		return addFlow;
	} else {
		return 0;
	}
}
