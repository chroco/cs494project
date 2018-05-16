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

#define PACKET_SIZE 	(1<<9)
#define MSG_SIZE 			PACKET_SIZE-sizeof(uint32_t)

typedef struct{
	uint32_t id;
	char msg[MSG_SIZE];
}IRCPacket;

typedef struct{
	void *data;
	int next;
	size_t size;
}Buffer;

class IRC{
	public:
		IRC();
		~IRC();

		void serializeIRCPacket(IRCPacket *dest,IRCPacket *src);
		void deserializeIRCPacket(IRCPacket *dest,IRCPacket *src);

		void uint32_t_to_char4(char *dst,uint32_t src);
		void char4_to_uin32_t(uint32_t *dst, char *src);
		void serialize_msg(char *dst,char *src);
		void deserialize_msg(char *dst,char *src);
	
	protected:
		char buffer[PACKET_SIZE];
		socklen_t addr_size;
		struct sockaddr_in serverAddr;
		int x,y,z;
};

#endif
