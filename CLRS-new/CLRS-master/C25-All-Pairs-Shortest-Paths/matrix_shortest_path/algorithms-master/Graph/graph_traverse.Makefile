graph_traverse: graph_traverse.o depth_first_search.o quickSort.o \
				breadth_first_search.o topological_search.o 
	cc -o graph_traverse graph_traverse.o depth_first_search.o \
			breadth_first_search.o topological_search.o quickSort.o

graph_traverse.o: graph_traverse.c graph_traverse.h macros.h
	cc -c -g graph_traverse.c 

depth_first_seach.o: depth_first_search.c graph_traverse.h macros.h
	cc -c -g depth_first_search.c

breadth_first_search.o: breadth_first_search.c graph_traverse.h
	cc -c -g breadth_first_search.c

topological_search.o: topological_search.c graph_traverse.h macros.h
	cc -c -g topological_search.c

quickSort.o: quickSort.c
	cc -c -g quickSort.c

.PHONY: clean
clean:
	rm -f graph_traverse *.o
