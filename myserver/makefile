.PHONY:clean all
SRC = $(wildcard *.cpp)
HEAD = $(wildcard HeaderFiles/*.h)
SRC1 = $(wildcard SourceFiles/*.cpp)
BIN = $(SRC:%.cpp=%)

all:$(BIN)

$(BIN):%:%.cpp $(SRC1)
	g++ $(CFLAGS) -std=c++11 $^ -o$@ -lpthread

CFLAGS=-g -Wall


clean:
	rm -f $(BIN)
