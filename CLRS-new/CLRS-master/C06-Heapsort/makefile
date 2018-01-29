sample : main.o p_queue.o
	g++ -std=c++11 -o sample main.o p_queue.o
main.o : main.cpp p_queue.h
	g++ -std=c++11 -c main.cpp
p_queue.o : p_queue.h p_queue.cpp
	g++ -std=c++11 -c p_queue.cpp

clean :
	rm sample main.o p_queue.o
