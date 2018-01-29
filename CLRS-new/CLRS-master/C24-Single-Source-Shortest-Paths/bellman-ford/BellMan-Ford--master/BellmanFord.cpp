
#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class Bellman{
	private:
		struct wEdge{
			int cost; std::string src; std::string dest;
			wEdge(int c, std::string s,std::string d){
				cost=c; dest=d; src=s;
			}
			wEdge(){}
		};
		std::string source;
    
        struct wVertex{
        std::string src;
        int distance;
        wVertex(std::string s){
            src=s;
        }
        wVertex(){}
    };
    
	public:
    // Create Vertex and Edge Vectors
	std::vector<wVertex> V_tracts;
	std::vector<wEdge> E_tracts;
		Bellman();
        void StorePrint();
        void BellmanFord();
	
};

// Read file through constructor 
Bellman::Bellman(){
	std::string line;
	char source;
	char incoming_input;
	int weight_input;
	
	while(std::getline(std::cin,line)){//read input file
		std::cout<<line<<std::endl;
		std::vector<std::string> parsed_lines;
		line.erase(std::remove(line.begin(),line.end(),' '),line.end());//remove all whitespace
		char *cline = new char[line.length() + 1];//convert to cstr in order tokenize the semicolons
        strcpy(cline, line.c_str());

        char * pLine;
        pLine = strtok(cline, ";");     //相当于正则表达式

        std::string source=pLine;
        wVertex vertex(source);//creates the vertex based on first input
        std::string fLine;
        
        while((pLine = strtok(NULL, ";"))!=NULL){
          //parses through the rest of the edges
        	fLine=pLine;
        	parsed_lines.emplace_back(fLine);//adds rest to list of edges
        }
        if(parsed_lines.size()==0){//makes sure vertex has edges
			V_tracts.emplace_back(vertex);
        	continue;
        }
        for(int i=0;i<parsed_lines.size();i++){
           //now to get rid of parenthesis and commas to look for dest. and cost.
			char *oldLine = new char[parsed_lines[i].length() + 1];
   	        strcpy(oldLine, parsed_lines[i].c_str());
   	        char *parLine;
   	        parLine = strtok(oldLine, ",");
   			std::string destVertex = parLine;
   			int loc = destVertex.find("(");
   			destVertex.erase(loc,1);// gets rid of parenthesis
   			//got the incoming, now for the cost
   			parLine = strtok(NULL, ","); //continue looking for cost
   			std::string costw=parLine;
   			loc = costw.find(")");
   			costw.erase(costw.length() - 1, 1); //erase ) at last position
   			int cost=atoi(costw.c_str());
   			wEdge edge(cost,source,destVertex);//adds new edge to vertex
   			E_tracts.emplace_back(edge);
        }
		V_tracts.emplace_back(vertex);
	}
}

//printfunction to print final Shortest distance from a single source
void Bellman::StorePrint(){
    
    for( auto it: E_tracts)
    {
      std::cout<<it.src <<"|---|" <<it.dest <<"|----|" <<it.cost <<std::endl;
    } 
    std::cout << std::endl;    
}

// Bellman Ford algorithm used to find shortes distance from a
// single source and also ditects negative weight cycle.
void Bellman::BellmanFord(){
 
    // set the first vertex as a single source
    source=V_tracts[0].src;
    for(int i=0;i<V_tracts.size();i++){
        if(V_tracts[i].src==source){
            V_tracts[i].distance=0;//set source distance to 0
        }
        else{
            V_tracts[i].distance=INT_MAX;//set other distances to "infitnity"
        }
        std::cout<<V_tracts[i].src<<": "<<V_tracts[i].distance <<std::endl;
    }
    std::cout<<"\n";
    
    /**
     Relax all edges |V| - 1 times. A simple shortest path from src
     to any other vertex can have at-most |V| - 1 edges
     **/
    for(int i=1;i<V_tracts.size()-1;i++){//go through and find initial shortest distance
        for(int j=0;j<E_tracts.size();j++){
            std::string src = E_tracts[j].src;
            std::string dest = E_tracts[j].dest;
            wVertex vsrc; wVertex vdest;
            int ksrc; int kdest;
            for(int k=0;k<V_tracts.size();k++){//find correct vertices
                if(src==V_tracts[k].src){
                    vsrc=V_tracts[k];
                    ksrc=k;
                }
                if(dest==V_tracts[k].src){
                    vdest=V_tracts[k];
                    kdest=k;
                }
            }
            int weight=E_tracts[j].cost;
            if(vsrc.distance!=INT_MAX&&vsrc.distance+weight<vdest.distance){//finds the shortest distance
                vdest.distance=vsrc.distance+weight;
                V_tracts[ksrc]=vsrc;
                V_tracts[kdest]=vdest;
                //std::cout<<V_tracts[ksrc].src<<"--->"<<V_tracts[kdest].src<<" cost: "<<weight<<std::endl;
                std::cout<<source<<" -----> "<<V_tracts[kdest].src<<" Distance: "<<vdest.distance<<std::endl<<std::endl;
                for(int k=0;k<V_tracts.size();k++){
                    std::cout<<V_tracts[k].src<<": "<<V_tracts[k].distance<<std::endl;
                }
            }
        }
    }
    
    /**
     check for negative-weight cycles, if graph doesn't contain negative weight cycle.
     print call to print the shortes path else: print  negative weight cycle detected
     **/ 
    bool found=false;
    for(int i=0;i<E_tracts.size();i++){
        std::string src = E_tracts[i].src;
        std::string dest = E_tracts[i].dest;
        wVertex vsrc; wVertex vdest;
        int ksrc; int kdest;
        for(int k=0;k<V_tracts.size();k++){//find correct vertices
            if(src==V_tracts[k].src){
                vsrc=V_tracts[k];
                ksrc=k;
            }
            if(dest==V_tracts[k].src){
                vdest=V_tracts[k];
                kdest=k;
            }
        }
        int weight=E_tracts[i].cost;
        if(vsrc.distance!=INT_MAX&&vsrc.distance+weight<vdest.distance){//finds the shortest distance
            std::cout<<"negative weigh Cycle found!"<<std::endl;
            found=true;
        }
    }
    if(!found){
        std::cout<<"No negative cycles."<<std::endl;
    }
    //print shortest distance 
    std::cout<<"Vertex  Shortest Distance" <<std::endl;
    for(auto is :V_tracts){
      std::cout<<is.src<<"|------|"<<is.distance<<std::endl;
    }
  return;
}// end of Bellman_Ford funtion 

// main funtion
int main(void){
    Bellman D;
    D.StorePrint();
    D.BellmanFord();
	
    return 0;
}
