#ifndef _IRC_H_
#define _IRC_H_

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#define BUFFER_SIZE (1<<10) 
#define HOME "127.0.0.1"
#define PORT 7891

typedef struct {
	int test[2];
} TestPacket;

class IRC{
	public:
		IRC();
		~IRC();

		template<class Archive>
		void serialize(Archive & archive);
		void testSerializer();
	
	protected:
		char buffer[BUFFER_SIZE];
		socklen_t addr_size;
		struct sockaddr_in serverAddr;
		int x,y,z;
};

#endif
