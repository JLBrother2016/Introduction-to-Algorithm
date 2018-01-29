#ifndef __ALL_PAIRS_SHORTEST_PATH_H__
#define __ALL_PAIRS_SHORTEST_PATH_H__

#define VER_MAX 100

struct Matrix_Graph {
	int node_num;
	double adjcent_matrix[VER_MAX][VER_MAX];
	double shortest_matrix[VER_MAX][VER_MAX];
	double tmp_matrix[VER_MAX][VER_MAX];
	int precusor[VER_MAX][VER_MAX];
	int tmp_precusor[VER_MAX][VER_MAX];
};

struct Edge {
	int to;
	double weight;
	double newWeight;
	struct Edge *next;
};

struct Node {
	int from;
	double distance;
	struct Edge *start;
};

struct Point_Graph {
	size_t node_num;
	struct Node node[VER_MAX];
};

void initGraph(struct Matrix_Graph *, struct Point_Graph *, int);
void input(struct Matrix_Graph *, struct Point_Graph *,  int);
void output(struct Matrix_Graph *);
void clearChoose(struct Matrix_Graph *, struct Point_Graph *);
void clearEdge(struct Point_Graph *);

#endif
