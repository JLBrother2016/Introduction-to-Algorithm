/*Name: kruskal
Compiler: Dev-C++ 5.8.0
Coded by: Jianyu Chen
Place: Middelburg, the Netherlands
Function: use kruskal algerithm to caculate the Minimum Spanning Tree
Time: 4th,Jan,2017 
*/
#include<iostream>
#include<vector>
using namespace std;
/////use struct to present the edge////
struct edge{
	int vertex1;
	int vertex2;
	int weight;
	edge(int vertex1,int vertex2,int weight);
};
edge::edge(int vertex1,int vertex2,int weight){
	this->vertex1=vertex1;
	this->vertex2=vertex2;
	this->weight=weight;
}


class kruskal{
	public:
		int num_point;                       /////////number of points
		int num_edge;                        ////number of edges
	    vector<edge> Edge,result;            ////the struct to store edges and Minimum Spanning Tree
        vector<bool> processed_vertex;       ////true means the vertex has been added to the result
        kruskal(int num_point);
        void addedge(int vertex1,int vertex2,int weight);
        void caculate(void);                 ///to caculate the Minimum Spanning Tree
        void show(void);
};
///////initialization
kruskal::kruskal(int num_point){
	this->num_point=num_point;
	num_edge=0;
	processed_vertex.resize(num_point);
	for(int i=0;i<num_point;i++){
		processed_vertex[i]=false;
	}
}
/////////////////add lines
void kruskal::addedge(int vertex1,int vertex2,int weight){
	edge newedge(vertex1,vertex2,weight);
	Edge.push_back(newedge);
	num_edge++;
}
void kruskal::caculate(void){
	edge n(0,0,0);
	int processed_point;     ///how many vertexes has been added
	processed_point=0;
/////////use bubble algerithm to sort the edges
	for(int i=num_edge-1;i>0;i--){
		for(int j=0;j<i;j++){
			if(Edge[j].weight>Edge[j+1].weight){
				n=Edge[j];
				Edge[j]=Edge[j+1];
				Edge[j+1]=n;
			}
		}
	}
	
///////choose the shortest pace
	processed_vertex[0]=true;   /////set the first vertex as processed 
	processed_point++;
	while(processed_point<num_point)
	for(int i=0;i<num_edge;i++){
		if((!processed_vertex[Edge[i].vertex1]) != (!processed_vertex[Edge[i].vertex2])){
			processed_vertex[Edge[i].vertex1]=true;
			processed_vertex[Edge[i].vertex2]=true;
			result.push_back(Edge[i]);
			processed_point++;
		}
	}
}
//////show the result. using the two vertexes to present edge 
void kruskal::show(void){
	for(vector<edge>::iterator iter=result.begin();iter!=result.end();++iter){
		cout<<iter->vertex1<<" "<<iter->vertex2<<endl;
	}
}
int main(void){
	kruskal mygraph(7);
	mygraph.addedge(0,1,7);      ////add edges to the graph
    mygraph.addedge(0,3,5);
	mygraph.addedge(3,5,6);	
	mygraph.addedge(1,3,9);
	mygraph.addedge(0,1,7);
	mygraph.addedge(3,4,15);
	mygraph.addedge(1,4,7);
	mygraph.addedge(1,2,8);
	mygraph.addedge(2,4,5);
	mygraph.addedge(4,6,9);
	mygraph.addedge(5,6,11);
	mygraph.caculate();         //////begin to caculate
	mygraph.show();
	return 0;
}
 
