.PHONY:clean all
SRC = $(wildcard *.cpp)
HEAD = $(wildcard HeaderFiles/*.h)
SRC1 = $(wildcard SourceFiles/*.cpp)
SRC2 = $(wildcard SourceFiles/*.c)
BIN = $(SRC:%.cpp=%)
BIN2 = $(SRC2:%.c=%.o)

all:$(BIN)

$(BIN2):%:$(SRC2)
	gcc $(CFLAGS) -c $^ -o $@ -lpthread
$(BIN):%:%.cpp $(SRC1) $(BIN2)
	g++ $(CFLAGS) -std=c++11 $^ -o $@ -lpthread

CFLAGS=-g -Wall


clean:
	rm -f $(BIN)
