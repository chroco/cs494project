all: epoll_example
 
epoll_example: epoll_example.c
	gcc -Wall -Werror -o $@ epoll_example.c

run:
	./epoll_example

clean:
	@rm -v epoll_example
