#include "server.h"

IRCServer::IRCServer(){}

IRCServer::~IRCServer(){}

/*
#define	MAXLINE	512

str_echo(sockfd)
int	sockfd;
{
	int	n;
	char	line[MAXLINE];

	for ( ; ; ) {
		n = readline(sockfd, line, MAXLINE);
		if (n == 0)
			return;		// connection terminated 
		else if (n < 0)
			perror("str_echo: readline error");

		if (writen(sockfd, line, n) != n)
			perror("str_echo: writen error");
	}
}
//*/
