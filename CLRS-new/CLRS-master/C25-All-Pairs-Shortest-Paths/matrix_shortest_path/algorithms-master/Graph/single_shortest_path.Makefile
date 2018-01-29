single_shortest_path: single_shortest_path.o bellman_ford.o \
						shortestpath_dag.o quickSort.o dijkstra.o \
						heapSort.o
	cc -o single_shortest_path single_shortest_path.o bellman_ford.o \
						shortestpath_dag.o quickSort.o dijkstra.o \
						heapSort.o

single_shortest_path.o: single_shortest_path.h single_shortest_path.c \
						macros.h
	cc -c -g -Wall single_shortest_path.c

bellman_ford.o: bellman_ford.c macros.h single_shortest_path.h
	cc -c -g -Wall bellman_ford.c

shortestpath_dag.o: shortestpath_dag.c single_shortest_path.h
	cc -c -g -Wall shortestpath_dag.c

dijkstra.o: dijkstra.c single_shortest_path.h
	cc -c -g -Wall dijkstra.c

quickSort.o: quickSort.c
	cc -c -g -Wall quickSort.c

heapSort.o: heapSort.c
	cc -c -g -Wall heapSort.c

.PHONY: clean
clean:
	rm -f single_shortest_path *.o
