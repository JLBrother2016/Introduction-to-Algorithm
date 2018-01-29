/*
delatvan@gmail.com
*/

#include <fstream> 
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "graph.hpp"
using namespace std;
 

 int main (int argc, char *argv[]){
 	
 	if (argc>1)
 	{
 		graph g(argv[1]);
 		g.edge_sort();
 	
 		vector<graph::edge> MST(g.number_vertices-1);
 		int edges_count=0;
 		long MST_weight = 0;

 		for(int i=0; i<g.edges.size();i++){
 			if(g.find(g.edges[i].left)!=g.find(g.edges[i].right)){
 				g.unionSet(g.edges[i].left,g.edges[i].right);
 				MST[edges_count]=g.edges[i];
 				MST_weight = MST_weight + MST[edges_count].cost;
 				edges_count++;
 			}
 		}
        cout << "output:" << endl;
 		if (edges_count == g.number_vertices-1) {
        	cout << "MSTGewicht: " << MST_weight << endl;
        	for (int k = 0; k < g.number_vertices-1; k++) {
            	cout << "Kante " << k << ": " << "{" << MST[k].left->iD << ", " << MST[k].right->iD << "}" << " mit Gewicht: " << MST[k].cost <<endl;
        	}
    	} 	
    	else {
        	cout << "Nicht zusammenhängend!" << endl;
    	}
 	}
 	

 	return 0;
 }


