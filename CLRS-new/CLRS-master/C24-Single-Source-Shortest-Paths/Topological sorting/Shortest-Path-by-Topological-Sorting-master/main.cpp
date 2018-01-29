//
//  Created by Jonas Anseeuw
//  Copyright (c) 2014 Jonas Anseeuw. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include "graph.h"
#define __STDC_LIMIT_MACROS
#include <limits.h>
// #include <stdint.h>
using namespace std;

/*#include <limits>
#include <cstdint>

struct X 
{ 
    static const std::int32_t i = std::numeric_limits<std::int32_t>::max(); 
};

int main()
{
    switch(std::numeric_limits<std::int32_t>::max()) { 
       case std::numeric_limits<std::int32_t>::max():
           std::cout << "this code works thanks to constexpr\n";
           break;
    }
    return EXIT_SUCCESS;
}*/


template<EdgeType ET, class Edgedata>
class WeightedGraph: public virtual GraphWithEdgedata<ET, Edgedata>{
public:
	WeightedGraph(int n=0):Graph<ET>(n){};
	// SHORTEST PATH FROM ONE NODE (uses private function to sort topological)
	void shortestPath(int s){
		// INIT VECTORS
		const int INFINITY = INT_MAX;
		std::vector<int> wt(this->numberOfVertices(), INFINITY); // Shortest paths
		std::vector<int> spt(this->numberOfVertices(), -1);
		wt[s] = 0;
		std::vector<int> topologicalOrder = this->topologicalSort();
		for(int i=0; i<topologicalOrder.size(); i++){
			int v = topologicalOrder[i];
			
			std::map<int, int>::iterator it = this->vertices[v].begin();
			if(wt[v] != INFINITY){
				while(it != this->vertices[v].end()){
					if(*(this->getEdgedata(v, it->first))+wt[v] < wt[it->first]){
						wt[it->first] = *(this->getEdgedata(v, it->first)) + wt[v];;
						spt[it->first] = v;
					}
					it++;
				}
			}
		}
		
		for(int i=0; i<this->numberOfVertices(); i++){
			(wt[i] == INFINITY) ? std::cout<<i<<": INFINITY"<<std::endl : std::cout<<i<<": "<<wt[i]<<std::endl;
		}
	}
private:
	std::vector<int> topologicalSort(){
		std::vector<int> ts(this->numberOfVertices());
		std::vector<int> indegrees(this->numberOfVertices());
		for(int i=0; i<this->numberOfVertices(); i++){
			std::map<int, int>::iterator it = this->vertices[i].begin();
			while(it != this->vertices[i].end()){
				indegrees[it->first]++;
				it++;
			}
		}
		std::queue<int> nextIndegree;
		for(int i=0; i<indegrees.size(); i++){
			if(indegrees[i] == 0){
				nextIndegree.push(i);
			}
		}
		int order=0;
		while(!nextIndegree.empty()){
			int v=nextIndegree.front();
			ts[order]=v;
			std::map<int ,int>::iterator it = this->vertices[v].begin();
			while(it != this->vertices[v].end()){
				indegrees[it->first]--;
				if(indegrees[it->first]==0){
					nextIndegree.push(it->first);
				}
				it++;
			}
			nextIndegree.pop();
			order++;
		}
		return ts;
	}
};

int main(int argc, char *argv[]){
	WeightedGraph<DIRECTED, int> g(6);
	g.addEdge(0, 1, 5);
	g.addEdge(0, 2, 3);
	g.addEdge(1, 3, 6);
	g.addEdge(1, 2, 2);
	g.addEdge(2, 4, 4);
	g.addEdge(2, 5, 2);
	g.addEdge(2, 3, 7);
	g.addEdge(3, 4, -1);
	g.addEdge(4, 5, -2);
	
	g.shortestPath(0);
	
    return 0;
}
