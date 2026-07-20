CC = gcc
CXX = g++

CFLAGS = -O3 -march=native -funroll-loops -DNDEBUG
CXXFLAGS = -O3 -march=native -funroll-loops -fopenmp -DNDEBUG

all: square cube power

square: square.cpp sprp64.o
	$(CXX) $(CXXFLAGS) square.cpp sprp64.o -o $@

cube: cube.cpp sprp64.o
	$(CXX) $(CXXFLAGS) cube.cpp sprp64.o -o $@

power: power.cpp sprp64.o
	$(CXX) $(CXXFLAGS) power.cpp sprp64.o -o $@

sprp64.o: sprp64.c sprp64.h
	$(CC) $(CFLAGS) -c sprp64.c -o sprp64.o

clean:
	rm -f square cube power sprp64.o

.PHONY: all clean