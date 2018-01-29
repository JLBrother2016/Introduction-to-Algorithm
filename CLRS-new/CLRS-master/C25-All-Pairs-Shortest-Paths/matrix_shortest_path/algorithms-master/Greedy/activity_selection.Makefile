activity_selection: activity_selection.o quickSort.o
	cc -o activity_selection activity_selection.o quickSort.o

activity_selection.o: activity_selection.c
	cc -c -Wall -g activity_selection.c

quickSort.o: quickSort.c macros.h
	cc -c -Wall -g quickSort.c

.PHONT: clean
clean:
	rm -f *.swp *.o activity_selection
