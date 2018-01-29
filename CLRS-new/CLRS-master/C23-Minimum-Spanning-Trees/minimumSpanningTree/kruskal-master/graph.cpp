/*
delatvan@gmail.com
*/

#include "graph.hpp"
#include <fstream> 
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

graph::graph(char const * filename)
{
	string buf;
	ifstream file(filename);

	if (file.is_open())
	{	
		
		getline(file,buf);
		stringstream s1(buf);
		s1>>number_vertices;
		vertices.resize(number_vertices);
		for (int i = 0; i < vertices.size(); ++i)  
		//In C++11 this for loop can be left out
		//If every vertex initiated with -1
		{
			vertices[i].iD=-1;
		}
		while(getline(file,buf)){
			stringstream ss(buf);
			int v_iD, u_iD, weight;
			ss>>v_iD >> u_iD >>weight;
			if (vertices[v_iD].iD==-1)
			{
				vertex v;
				v.iD= v_iD;
				v.rank=0;
				v.next = &(vertices[v_iD]);
				vertices[v_iD]= v;
			}
			if (vertices[u_iD].iD==-1)
			{
				vertex u;
				u.iD= u_iD;
				u.rank=0;
				u.next = &(vertices[u_iD]);
				vertices[u_iD]= u;
			}
			edge e;
			e.cost = weight;
			e.left = &(vertices[v_iD]);
			e.right = &(vertices[u_iD]);
			edges.push_back(e);//
			

		}
	}
	file.close();
}


graph::~graph()
{}


void graph::print()
{	cout<<"Edges"<<endl;
	for(int i=0; i<edges.size(); i++){
		cout<<(edges[i].left)->iD<<" "<<(edges[i].right)->iD<<" "<<edges[i].cost<<endl;
	}
	cout<<"vertices"<<endl;
	for(int i=0; i<vertices.size(); i++){
		cout<<vertices[i].iD<<endl;
	}
}

void graph::edge_sort()
{
	edge_cmp comparison;
	sort(edges.begin(),edges.end(),comparison);    //实际是快排
}

graph::vertex* graph::find(vertex* v) {
    if (v->next != v) {
        v->next = find(v->next);
    }
    return v->next;
}

bool graph::unionSet(vertex* v, vertex* u) {

    vertex* x = find(v);
    vertex* y = find(u);

    if (x->rank > y->rank) {
        y->next = x;
    } else {
        x->next = y;
        if (y->rank == x->rank) {
            y->rank++;
        }
    }

    return true;
}
