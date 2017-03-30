.PHONY:clean all
SRC = $(wildcard xsserver/*.cpp)
HEAD = $(wildcard xsserver/HeaderFiles/*.h)
SRC1 = $(wildcard xsserver/SourceFiles/*.cpp)
BIN = $(SRC:%.cpp=%)

all:$(BIN)

$(BIN):%:%.cpp
	g++ $(CFLAGS) -std=c++11 $^ -o$@

CFLAGS=-g -Wall


clean:
	rm -f $(BIN)
