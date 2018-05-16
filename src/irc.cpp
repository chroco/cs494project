#include "irc.h"

IRC::IRC():buffer{0},addr_size(0){
	memset(&serverAddr, 0, sizeof(serverAddr));
}

IRC::~IRC(){}

void IRC::serializeIRCPacket(char *dest,IRCPacket *src){
  uint32_t_to_char4(dest,src->id);
  serialize_msg(dest+sizeof(uint32_t),src->msg);
}

void IRC::deserializeIRCPacket(IRCPacket *dest,char *src){
  char4_to_uin32_t(&dest->id,src);
  deserialize_msg(dest->msg,src+sizeof(uint32_t));
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
	for(;i <= MSG_SIZE; i += sizeof(uint32_t)){
		p_uint32_t = reinterpret_cast<uint32_t *>(dst+i);
		char4_to_uin32_t(p_uint32_t,&src[i]);
	}
}

void IRC::deserialize_msg(char *dst,char *src){
	uint32_t i = 0;
	for(;i <= MSG_SIZE;i += sizeof(uint32_t)){
		uint32_t_to_char4(&dst[i],(uint32_t &)src[i]);
	}
}

