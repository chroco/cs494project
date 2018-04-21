#include "server.h"

int main(int argc,char *argv[]){
	IRCServer *pServer = new IRCServer();
	pServer->helloSocket();
	//pServer->testSerializer();

	delete pServer;
	return 0;
}
