#include "irc.h"

IRC::IRC():buffer{0},addr_size(0){
	memset(&serverAddr, 0, sizeof(serverAddr));
}

IRC::~IRC(){}

void IRC::serializeIRCPacket(IRCPacket *dest,IRCPacket *src){
  serialize_uint32_t(&dest->id,src->id);
//  serialize_msg(dest->msg, src->msg);
}

void IRC::deserializeIRCPacket(IRCPacket *dest,IRCPacket *src){
  deserialize_uint32_t(&dest->id,src->id);
  deserialize_msg(dest->msg, src->msg);
}

void IRC::serialize_uint32_t(uint32_t *dest,uint32_t src) {
	src = htonl(src);
	memcpy(dest,&src,sizeof(uint32_t));
}

void IRC::deserialize_uint32_t(uint32_t *dest,uint32_t src) {
	memcpy(dest,&src,sizeof(uint32_t));
	*dest = ntohl(*dest);
}

void IRC::serialize_msg(char *dest,char *src) {
	uint32_t i=0;
	uint32_t temp;
	printf("serialize_msg:\n");
	for(;i<MSG_SIZE && src[i] != '\0';i=i+sizeof(uint32_t)){
		printf("%c",src[i]);
		temp = htonl(src[i]);
		memcpy(&dest[i],&temp,sizeof(uint32_t));
		printf("%u\n",temp);
		printf("%u\n",dest[i]);
	}
	printf("\n");
//	memcpy(dest,src,sizeof(char)*i);

}

void IRC::deserialize_msg(char *dest,char *src) {
	uint32_t i=0;
	char temp;
	for(;i<MSG_SIZE && src[i] != '\0';++i){
	//	src[i] = ntohl(src[i]);
		temp = ntohl(src[i]);
		memcpy(&dest[i],&temp,sizeof(char));
	}
//	memcpy(dest,src,sizeof(char)*i);
}

template<class Archive>
void IRC::serialize(Archive & archive){
	archive( x, y, z ); // serialize things by passing them to the archive
}

void IRC::testSerializer(){
  cereal::JSONOutputArchive archive( std::cout );
  bool arr[] = {true, false};
  std::vector<int> vec = {1, 2, 3, 4, 5};
  archive( CEREAL_NVP(vec),arr );
}
