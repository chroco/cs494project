#ifndef _IRC_H_
#define _IRC_H_

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>	
#include <netinet/in.h>	
#include <netdb.h>			
#include <sys/time.h>		
#include <unistd.h>			

#include <iostream>

#define	BUFLEN	1024	/* buffer length 	   */

#define HOME "127.0.0.1"
#define PORT 7891

#define MSG_SIZE	int((1<<10) - 3*sizeof(uint32_t))			
#define CMD_SIZE	10	

typedef struct {
	uint32_t length;
	uint32_t op_code;
	uint32_t error_code;
	char msg[MSG_SIZE];
}packet;

#define PACKET_SIZE	sizeof(packet) 
#define BUFFER_SIZE PACKET_SIZE 

enum{
	PING=0,
	JOIN,
	PART,
	LIST,
	NICK,
	MSG,
	END_OF_COMMANDS
};

//const char *command[];

typedef union{
	packet p;
	char serial[PACKET_SIZE];
}IRCPacket;

#define IRC_PACKET_SIZE sizeof(IRCPacket)

extern const char *command[];

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
