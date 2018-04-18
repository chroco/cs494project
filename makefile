SERVER_BIN=irc_server
CLIENT_BIN=irc_client
CC=g++ -std=c++11
CCFLAGS=-I ./include -g -Wall -Wextra -std=c++11
CCFLAGS+= -Wno-unused-parameter
LDFLAGS=

SRCS=$(wildcard ./src/*.cpp) $(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(SERVER_BIN) $(CLIENT_BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o:%.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(BIN)

valgrind: $(BIN)
	valgrind --leak-check=full \
		./$(BIN)
