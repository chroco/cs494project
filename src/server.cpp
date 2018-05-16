#include "server.h"
//#include "channel.h"

IRCServer::IRCServer():pChannels(NULL),welcomeSocket(0),newSocket(0){
	memset(&serverStorage, 0, sizeof(serverStorage));
	pChannels = new ChannelList();
}

IRCServer::~IRCServer(){
//	delete pChannels;
}

void IRCServer::welcome(){
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr(HOME);
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

	std::stringstream ss;

	cereal::JSONOutputArchive oarchive( ss );
 // bool arr[] = {true, false};
 // std::vector<int> vec = {1, 2, 3, 4, 5};
 // oarchive( CEREAL_NVP(vec),arr );
	TestPacket test;
	test.test[0]=9;
	test.test[1]=0;
  oarchive(test.test[0],test.test[1]);

	strcpy(buffer, ss.str().c_str());
  send(newSocket,buffer,BUFFER_SIZE,0);
}


void IRCServer::getStats(){
	printf("sizeof(IRCPacket): %zu\n",sizeof(IRCPacket));
	printf("sizeof(buffer): %zu\n",sizeof(buffer));
	printf("sizeof(char)*PACKET_SIZE: %zu\n",sizeof(char)*PACKET_SIZE);
}

void IRCServer::helloSocket(){
  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(PORT);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr(HOME);
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  /*---- serialize ----*/

	IRCPacket test = {42u,{"hello test packet!\0"}};
	uint32_t uint32_t_buf=0;
	char sendbuf[MSG_SIZE]={0};
	char testbuf[MSG_SIZE]={0};
//	serialize_uint32_t(&uint32_t_buf,test.id);
	printf("%s\n",test.msg);
	serialize_msg(sendbuf,test.msg);
	deserialize_msg(testbuf,sendbuf);
  printf("serialized data: %s\n",testbuf);
	//serializeIRCPacket(&send_buf,&test);

  /*---- Send message to the socket of the incoming connection ----*/

	send(newSocket,sendbuf,sizeof(char)*MSG_SIZE,0);
	//send(newSocket,&uint32_t_buf,PACKET_SIZE,0);
	//send(newSocket,&send_buf,PACKET_SIZE,0);

}
/*
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    unsigned int number = 175;

    unsigned int number2 = htonl(number);
    char numberStr[4];
    memcpy(numberStr, &number2, 4);

    printf("%x %x %x %x\n", numberStr[0], numberStr[1], numberStr[2], numberStr[3]);

    return 0;
}
//*/
