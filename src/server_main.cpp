#include "server.h"

int main(int argc,char *argv[]){
	IRCServer *pServer = new IRCServer();
	pServer->welcome();

	delete pServer;
	fprintf(stderr,"delete pServer executed\n");
	return 0;
}

