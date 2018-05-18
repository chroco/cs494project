#include "server.h"

int main(int argc,char *argv[]){
	IRCServer *pServer = new IRCServer();
	pServer->getStats();
	pServer->helloSocket();
//	pServer->welcome();

	delete pServer;
	return 0;
}

/*
	bind(welcomeSocket, (struct sockAddr *) &serverAddr, sizeof(serverAddr));

  if(listen(welcomesocket,5)==0)
    printf("listening\n");
  else
    printf("error\n");

  addr_size = sizeof serverstorage;
  newsocket = accept(welcomeSocket, (struct sockaddr *) &serverstorage, &addr_size);


	ircpacket test = {42u,{"hello test packet!\0"}};
	char sendbuf[packet_size]={0};
//	printf("%s\n",test.msg);
	//serialize_msg(sendbuf,test.msg);	
	serializeIRCPacket(sendbuf,test);	
//  printf("serialized data: %s\n",sendbuf);


	send(newsocket,sendbuf,sizeof(PACKET_SIZE),0);
	//send(newSocket,sendbuf,sizeof(char)*MSG_SIZE,0);


*/
