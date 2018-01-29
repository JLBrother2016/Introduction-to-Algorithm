#ifndef __MAXIMUM_FLOW_H__
#define __MAXIMUM_FLOW_H__

#define VER_MAX 100

struct Graph {
	int ver_num;
	int src_no;
	int dst_no;
	int precusor[VER_MAX];
	char visited[VER_MAX];
	double capacity[VER_MAX][VER_MAX];
	double resiCapacity[VER_MAX][VER_MAX];
	double flow[VER_MAX][VER_MAX];
};

struct Queue {
	int queue[VER_MAX];
	int head;
	int tail;
};

void initGraph(struct Graph *, int, int, int);
void input(struct Graph *, int);
void bfs(struct Graph *);

#endif
