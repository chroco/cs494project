#ifndef _IRC_H_
#define _IRC_H_

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#define HOME "127.0.0.1"
#define PORT 7891

typedef struct {
	int test[2];
} TestPacket;

#define MSG_SIZE		(1<<9) - sizeof(uint32_t)			

typedef struct {
		uint32_t id;
		char msg[MSG_SIZE];
}packet;

#define PACKET_SIZE	sizeof(packet) 
#define BUFFER_SIZE PACKET_SIZE 

typedef union{
	packet p;
	char serial[PACKET_SIZE];
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

		void serializeIRCPacket(char *dest,IRCPacket *src);
		void deserializeIRCPacket(IRCPacket *dest,char *src);


		void serialize_packet(char *dst,char *src);
		void deserialize_packet(char *dst,char *src);

		void serialize_uint32_t(uint32_t *dst,uint32_t src);
		void deserialize_uint32_t(uint32_t *dst,uint32_t src);
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
