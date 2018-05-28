CC = gcc
CFLAGS = -Wall -g

default: client server
	echo "compiled!"

client: echoclient.c echo.h
	$(CC) -o client echoclient.c $(CFLAGS)
	
server: echoserver.c echo.h
	$(CC) -o server echoserver.c $(CFLAGS)

clean:
	rm client server
	
