#include "server.h"
//#include "channel.h"

IRCServer::IRCServer():pChannels(NULL),welcomeSocket(0),newSocket(0){
	memset(&serverStorage, 0, sizeof(serverStorage));
	pChannels = new ChannelList();
	pClients = new ClientList();
}

IRCServer::~IRCServer(){
	delete pChannels;
	delete pClients;
}

void IRCServer::welcome(){
  int bytes_sent=0;
	char sendbuf[5] = "ack\0";
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr(HOME);
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	
	
	while(listen(welcomeSocket,5)==0){
    printf("Listening\n");
  	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
  	addr_size = sizeof serverStorage;
    printf("Request received, adding client\nsocket: %d\n",newSocket);
		pClients->addClient(newSocket);
		pClients->printList();
/*
		bytes_sent=send(newSocket,sendbuf,5,0);
		if(bytes_sent < 0){
			fprintf(stderr, "error sending...");
			return;
		}
		fprintf(stderr, "bytes_sent: %d\n",bytes_sent);
//*/
		newSocket = 0;
	}

//  send(newSocket,buffer,BUFFER_SIZE,0);
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
//	bind(welcomeSocket, (struct sockAddr *) &serverAddr, sizeof(serverAddr));
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  if(listen(welcomeSocket,5)==0)
    printf("listening\n");
  else
    printf("error\n");

  /*---- accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  /*---- serialize ----*/

	IRCPacket test = {42u,{"hello test packet!\0"}},
						des_test = {0,0};
	char sendbuf[IRC_PACKET_SIZE]={0};
	printf("%u\n%s\n",test.p.id,test.p.msg);
	serializeIRCPacket(sendbuf,&test);	
	deserializeIRCPacket(&des_test,sendbuf);	
	printf("%u\n%s\n",des_test.p.id,des_test.p.msg);
  /*---- send message to the socket of the incoming connection ----*/
	int bytes_sent = 0;
	bytes_sent=send(newSocket,sendbuf,IRC_PACKET_SIZE,0);
	
	if(bytes_sent < 0){
		fprintf(stderr, "error sending...");
		return;
	}
	fprintf(stderr, "bytes_sent: %d\n",bytes_sent);
}

