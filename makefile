SERVER_BIN=irc_server
CLIENT_BIN=irc_client
SRC=./src
CC=g++ -std=c++11
CCFLAGS=-I ./include -g -Wall -Wextra -std=c++11
CCFLAGS+= -Wno-unused-parameter
LDFLAGS=

SRCS=$(SRC)/irc.c
SERVER_SRCS=$(SRC)/server_main.c $(SRC)/server.c
CLIENT_SRCS=$(SRC)/client_main.c $(SRC)/server.c
#SRCS=$(wildcard ./src/*.cpp) $(wildcard *.cpp)

SERVER_OBJS=$(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS=$(CLIENT_SRCS:.cpp=.o)

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_OBJS)
	$(CC) $(SEVER_OBJS) -o $@ $(LDFLAGS)

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $@ $(LDFLAGS)

%.o:%.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(BIN)

valgrind: $(BIN)
	valgrind --leak-check=full \
		./$(SERVER_BIN)
		./$(CLIENT_BIN)
