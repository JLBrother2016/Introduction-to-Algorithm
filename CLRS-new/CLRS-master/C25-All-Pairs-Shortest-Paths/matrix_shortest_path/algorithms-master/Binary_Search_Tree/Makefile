test: test.o binary_search_tree.o 
	cc -o test test.o binary_search_tree.o

test.o: test.c binary_search_tree.h
	cc -c -g test.c binary_search_tree.h

binary_search_tree.o: binary_search_tree.h binary_search_tree.c
	cc -c -g binary_search_tree.h binary_search_tree.c

.PHONY: clean
clean:
	rm -f test *.o *~ *.gch
