#include <stdio.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>

#define MSG_SIZE (1<<9)

void uint32_t_to_char4(char *dst,uint32_t src){
	uint32_t num = htonl(src);
	memcpy(dst, &num, sizeof(uint32_t));
}

void char4_to_uin32_t(uint32_t *dst, char *src){
	uint32_t num = 0;
	memcpy(&num, src, sizeof(uint32_t));
	*dst = ntohl(num);
}

void serialize_msg(char *dst,char *src){
	uint32_t i = 0, *p_uint32_t = NULL;
	for(;i <= MSG_SIZE; i += sizeof(uint32_t)){
		p_uint32_t = reinterpret_cast<uint32_t *>(dst+i);
		char4_to_uin32_t(p_uint32_t,&src[i]);
	}
}

void deserialize_msg(char *dst,char *src){
	uint32_t i = 0;
	for(;i <= MSG_SIZE;i += sizeof(uint32_t)){
		uint32_t_to_char4(&dst[i],(uint32_t &)src[i]);
	}
}

void test_things(){
	uint32_t number = 187;
	char word[4];
	printf("%u\n",number);
	uint32_t_to_char4(word,number);
	number = 0;
	printf("%x %x %x %x\n", word[0], word[1], word[2], word[3]);
	char4_to_uin32_t(&number,word);
	printf("%u\n",number);
}

void test_msg_things(){
	char msg[MSG_SIZE] = {"Hello Serializer!\0"},
			 ser[MSG_SIZE] = {0},
			 res[MSG_SIZE] = {0};
	printf("msg: %s\n",msg);
	serialize_msg(ser,msg);
	printf("ser: %s\n",ser);
	deserialize_msg(res,ser);
	printf("res: %s\n",res);
}

int main() {
	test_things();
	test_msg_things();
	return 0;
}
