all: kruskal

kruskal: main.o graph.o
	g++ main.o graph.o -o kruskal

main.o: main.cpp graph.hpp
	g++ -c main.cpp 

graph.o: graph.cpp graph.hpp
	g++ -c graph.cpp

clean:
	rm -rf kruskal *.o




