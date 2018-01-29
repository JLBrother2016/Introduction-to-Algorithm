maximum_flow: maximum_flow.o ford_fulkerson.o
	cc -o maximum_flow maximum_flow.o ford_fulkerson.o

maximum_flow.o: maximum_flow.h maximum_flow.c
	cc -c -g -Wall maximum_flow.c

ford_fulkerson.o: ford_fulkerson.c maximum_flow.h
	cc -c -g -Wall ford_fulkerson.c

.PHONY: clean
clean:
	rm -f *.o maximum_flow
