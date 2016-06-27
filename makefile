all: main.o serial.o tester.o printer.o
	g++ -o main -g main.o serial.o tester.o printer.o -Wall -lboost_system
clean:
	rm *.o main
