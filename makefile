all: main.o serial.o
	g++ -o main main.o serial.o -Wall -lboost_system
clean:
	rm *.o main

