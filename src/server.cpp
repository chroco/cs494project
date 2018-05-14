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
  /*---- Send message to the socket of the incoming connection ----*/
//  strcpy(buffer,"Hello Socket!\n");
//  send(newSocket,buffer,sizeof("Hello Socket!\n")/sizeof(char),0);
  send(newSocket,buffer,BUFFER_SIZE,0);

}


