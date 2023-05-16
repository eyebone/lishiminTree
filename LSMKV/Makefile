CC = g++
LINK.o = $(LINK.cc)
CXXFLAGS = -std=c++14 -Wall -g

objects = kvstore.o correctness.o MemTable.o
#CFLAGS = -g -Wall -O2

all: correctness persistence

correctness: kvstore.o correctness.o MemTable.o

persistence: kvstore.o persistence.o MemTable.o

clean:
	-rm -f correctness persistence *.o