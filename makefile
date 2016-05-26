all: main.o serialmin.o
	g++ -o main main.o serialmin.o -Wall -lboost_system
clean:
	rm *.o main

