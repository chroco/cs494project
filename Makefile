CC=g++ -std=c++11
SERVER_BIN=server 
CLIENT_BIN=client 
CLIENT_CFLAGS=
SERVER_CFLAGS=
FLAGS=-I ./include -g -Wall -Wextra -std=c++11 -Wno-unused-parameter
INC= ./include
SRC= ./src
OBJ=./src/obj
SERVER_SRCS=$(SRC)/server_main.cpp $(SRC)/server.cpp $(SRC)/irc.cpp
CLIENT_SRCS=$(SRC)/client_main.cpp $(SRC)/client.cpp $(SRC)/irc.cpp
SERVER_INC=$(INC)/server.h $(INC)/irc.h
CLIENT_INC=$(INC)/client.h $(INC)/irc.h
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

default: $(SERVER_BIN) $(CLIENT_BIN)
	@echo $(SERVER_BIN) and $(CLIENT_BIN) compiled
 
#server: $(SERVER_BIN)
#	@echo $(SERVER_BIN) compiled

$(SERVER_BIN): $(SERVER_OBJS)
	$(CC) $(SERVER_CFLAGS) -o $(SERVER_BIN) $(SERVER_OBJS)

#client: $(CLIENT_BIN)
#	@echo $(CLIENT_BIN) compiled

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(CC) $(CLIENT_CFLAGS) -o $(CLIENT_BIN) $(CLIENT_OBJS)

.cpp.o: 
	$(CC) $(FLAGS) -c $^ -o $@

clean: 
	rm -f $(SRC)/*.o *.o $(SERVER_BIN) $(CLIENT_BIN)

valgrind: $(BIN)
	valgrind --leak-check=full \
		./$(SERVER_BIN)
		./$(CLIENT_BIN)
