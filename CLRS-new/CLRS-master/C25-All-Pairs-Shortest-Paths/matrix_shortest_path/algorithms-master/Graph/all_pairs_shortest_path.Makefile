all_pairs_shortest_path: all_pairs_shortest_path.o matrix_multiplication.o \
						floyd_warshall.o johnson.o heapSort.o
	cc -o all_pairs_shortest_path all_pairs_shortest_path.o heapSort.o \
			matrix_multiplication.o floyd_warshall.o johnson.o

all_pairs_shortest_path.o: all_pairs_shortest_path.h \
							all_pairs_shortest_path.c \
							macros.h
	cc -c -g -Wall all_pairs_shortest_path.c

matrix_multiplication.o: all_pairs_shortest_path.h macros.h \
						matrix_multiplication.c
	cc -c -g -Wall matrix_multiplication.c

floyd_warshall.o: floyd_warshall.c all_pairs_shortest_path.h
	cc -c -g -Wall floyd_warshall.c

johnson.o: johnson.c macros.h all_pairs_shortest_path.h
	cc -c -g -Wall johnson.c

heapSort.o: heapSort.c
	cc -c -g -Wall heapSort.c

.PHONY:clean
clean:
	rm all_pairs_shortest_path *.o
