#include "irc.h"

IRC::IRC():buffer{0},addr_size(0){
	memset(&serverAddr, 0, sizeof(serverAddr));
}

IRC::~IRC(){}
