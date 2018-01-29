#ifndef __GRAPH_TRAVERSE__
#define __GRAPH_TRAVERSE__

#define VER_MAX 100

struct Edge {
	int to;
	struct Edge *next;
};

struct Node {
	int from;
	int start_time;
	int end_time;
	char visited;
	struct Edge *start;
};

struct Graph {
	int node_num;
	struct Node node[VER_MAX];
};

void initGraph(struct Graph *, int);
void input(struct Graph *, int);
void visitNode(struct Graph *, int);
void clearVisit(struct Graph *);
void deleteEdge(struct Graph *);

#endif
