#ifndef _IRC_H_
#define _IRC_H_

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE (1<<10) 
#define HOME "127.0.0.1"
#define PORT 7891

class IRC{
	public:
		IRC();
		~IRC();

	protected:
		char buffer[BUFFER_SIZE];
		socklen_t addr_size;
		struct sockaddr_in serverAddr;
};

#endif
