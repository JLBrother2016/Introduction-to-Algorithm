main: main.o gtest.o
	g++ -o main main.o gtest.o -lpthread -std=c++14
	rm *.o
main.o: main.cpp
	g++ -o main.o -c main.cpp -lpthread -std=c++14
gtest.o: ./src/google_test/gtest-all.cc
	g++ -o gtest.o -c ./src/google_test/gtest-all.cc -lpthread -std=c++14

clean: 
	rm -f *.o
