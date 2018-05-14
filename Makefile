CC=g++ -std=c++11

SERVER_BIN=server 
CLIENT_BIN=client
TEST_BIN=test

CLIENT_CFLAGS=
SERVER_CFLAGS=
TEST_CFLAGS=
FLAGS=-I ./include -g -Wall -Wextra \
			-std=c++11 -Wno-unused-parameter

INC= ./include
SRC= ./src
OBJ= ./src/obj

SERVER_SRCS=$(SRC)/server_main.cpp \
						$(SRC)/server.cpp \
						$(SRC)/irc.cpp \
						$(SRC)/dll.cpp \
						$(SRC)/channel.cpp \
						$(SRC)/node.cpp

CLIENT_SRCS=$(SRC)/client_main.cpp \
						$(SRC)/client.cpp \
						$(SRC)/irc.cpp

TEST_SRCS=$(SRC)/test_main.cpp \
					$(SRC)/test.cpp \
					$(SRC)/dll.cpp \
					$(SRC)/channel.cpp \
					$(SRC)/node.cpp

SERVER_INC=$(INC)/server.h \
					 $(INC)/irc.h \
					 $(INC)/dll.h	\
					 $(INC)/channel.h	\
					 $(INC)/node.h

CLIENT_INC=$(INC)/client.h \
					 $(INC)/irc.h

TEST_INC=$(INC)/test.h \
				 $(INC)/dll.h	\
				 $(INC)/channel.h	\
				 $(INC)/node.h

SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

default: $(SERVER_BIN) $(CLIENT_BIN) $(TEST_BIN)
	@echo build successful!
 
$(SERVER_BIN): $(SERVER_OBJS)
	$(CC) $(SERVER_CFLAGS) -o $(SERVER_BIN) $(SERVER_OBJS)
	@echo $(SERVER_BIN) compiled!

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(CC) $(CLIENT_CFLAGS) -o $(CLIENT_BIN) $(CLIENT_OBJS)
	@echo $(CLIENT_BIN) compiled!

$(TEST_BIN): $(TEST_OBJS)
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN) $(TEST_OBJS)
	@echo $(TEST_BIN) compiled!

.cpp.o: 
	$(CC) $(FLAGS) -c $^ -o $@

run_server:	run_test
	./$(SERVER_BIN)

run_client: run_test
	./$(CLIENT_BIN)

run_test:
	valgrind --leak-check=full \
	./$(TEST_BIN)

clean: 
	rm -f $(SRC)/*.o \
		*.o \
		$(SERVER_BIN) \
		$(CLIENT_BIN) \
		$(TEST_BIN)

valgrind: $(BIN)
	valgrind --leak-check=full \
		./$(SERVER_BIN)
		./$(CLIENT_BIN)
		./$(TEST_BIN)
