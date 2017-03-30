.PHONY:clean all
SRC = $(wildcard *.cpp)
BIN = $(SRC:%.cpp=%)

all:$(BIN)

$(BIN):%:%.cpp
	g++ $(CFLAGS) -std=c++11 $^ -o$@

CC=gcc
CFLAGS=-g -Wall


clean:
	rm -f $(BIN)
