#include "server.h"

int main(int argc,char *argv[]){
	IRCServer *pServer = new IRCServer();
//	pServer->getStats();
//	pServer->helloSocket();
	pServer->welcome();
//	pServer->select_welcome();

	delete pServer;
	return 0;
}

