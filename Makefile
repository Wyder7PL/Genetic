all: main.o
	g++ -std=c++11 main.o -o genetic 

main.o: main.cpp main.h
	g++ -c -o $@ $< -std=c++11

clean:
	rm -f *.o exe