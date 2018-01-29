/*
delatvan@gmail.com
*/


// graph.hpp (Declaration of Class Graph)
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>

using namespace std;


class graph{

public:

graph(char const* filename);
~graph();

struct vertex {
	int iD; //for C++11 use instead int iD =-1;
	int rank;
	vertex *next;
};


struct edge {
	int cost;
	vertex *left;
	vertex *right;
};

struct edge_cmp{
	bool operator()(const edge& e1, const edge& e2) const{
		return e1.cost<e2.cost;
	}
};

vector<vertex> vertices;
vector<edge> edges;
int number_vertices;

void print();

void edge_sort();

vertex* find(vertex* v);

bool unionSet(vertex* v, vertex* u);

private:

};

#endif