#include "irc.h"

IRC::IRC():buffer{0},addr_size(0){
	memset(&serverAddr, 0, sizeof(serverAddr));
}

IRC::~IRC(){}

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
