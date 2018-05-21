#include "client.h"

int main(int argc,char *argv[]){
	IRCClient *pClient = new IRCClient();
//	pClient->helloSocket();
	pClient->requestConnection();
	delete pClient;
	return 0;
}
