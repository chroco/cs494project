#include "irc.h"

const char *command[] = {
	"/ping",
	"/join",
	"/part",
	"/list",
	"/nick",
	"/msg",
	"/exit"
};

IRC::IRC():buffer{0},addr_size(0){
	memset(&serverAddr, 0, sizeof(serverAddr));
}

IRC::~IRC(){}

void IRC::serializeIRCPacket(char *dst,IRCPacket *src){
	serialize_packet(dst,src->serial);
}

void IRC::serialize_packet(char *dst,char *src){
	uint32_t i = 0, *p_uint32_t = NULL;
	for(;i < IRC_PACKET_SIZE; i += sizeof(uint32_t)){
		p_uint32_t = reinterpret_cast<uint32_t *>(dst+i);
		char4_to_uin32_t(p_uint32_t,&src[i]);
	}
}

void IRC::deserializeIRCPacket(IRCPacket *dst,char *src){
	deserialize_packet(dst->serial,src);
}

void IRC::deserialize_packet(char *dst,char *src){
	uint32_t i = 0;
	for(;i < IRC_PACKET_SIZE;i += sizeof(uint32_t)){
		uint32_t_to_char4(&dst[i],(uint32_t &)src[i]);
	}
}

void IRC::serialize_uint32_t(uint32_t *dst,uint32_t src){
	uint32_t num = htonl(src);
	memcpy(dst, &num, sizeof(uint32_t));
}

void IRC::deserialize_uint32_t(uint32_t *dst,uint32_t src){
	uint32_t num = ntohl(src);
	memcpy(dst, &num, sizeof(uint32_t));
}

void IRC::uint32_t_to_char4(char *dst,uint32_t src){
	uint32_t num = htonl(src);
	memcpy(dst, &num, sizeof(uint32_t));
}

void IRC::char4_to_uin32_t(uint32_t *dst, char *src){
	uint32_t num = 0;
	memcpy(&num, src, sizeof(uint32_t));
	*dst = ntohl(num);
}

void IRC::serialize_msg(char *dst,char *src){
	uint32_t i = 0, *p_uint32_t = NULL;
	for(;i < MSG_SIZE; i += sizeof(uint32_t)){
		p_uint32_t = reinterpret_cast<uint32_t *>(dst+i);
		char4_to_uin32_t(p_uint32_t,&src[i]);
	}
}

void IRC::deserialize_msg(char *dst,char *src){
	uint32_t i = 0;
	for(;i < MSG_SIZE;i += sizeof(uint32_t)){
		uint32_t_to_char4(&dst[i],(uint32_t &)src[i]);
	}
}

int IRC::truncateFirstWord(char *word,char *message,int size){
	int i=0,j=0;
//	printf("extracting first word in message: %s\n",message);
	for(;i<size && message[i]!=' ' && message[i]!='\0';++i){
		word[i]=message[i];	
	}
	++i;
	word[i]='\0';
//	printf("extracetd word: %s\n",word);

//	printf("truncating first word in message: %s\n",message);
	for(;i<MSG_SIZE && message[i]!='\0';++i,++j){
		message[j]=message[i];	
	}
	message[j]='\0';
//	printf("message after truncation: %s\n",message);
	return 0;
}
