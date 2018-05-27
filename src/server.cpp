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
	IRCPacket test = {0u,0u,0u,{"lolololololz!\0"}},
						irc_msg = {0,0,0,0};
	char sendbuf[IRC_PACKET_SIZE]={0},
			 recvbuf[IRC_PACKET_SIZE]={0};
	
	int	i,rc,s,cs,dsize; 
//	char buf[BUFLEN+1];  
	struct sockaddr_in sa; 
	struct sockaddr_in csa;
	socklen_t size_csa;
	fd_set rfd; 	
	fd_set c_rfd; 

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	sa.sin_addr.s_addr = INADDR_ANY;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		perror("socket: allocation failed");
	}

	rc = bind(s,(struct sockaddr *)&sa,sizeof(sa));

	if (rc) {
		perror("bind");
	}

	printf("listening...\n");
	rc = listen(s, 5);

	if (rc) {
		perror("listen");
	}

	size_csa = sizeof(csa);
	dsize = getdtablesize();
	
	for (i=3; i<dsize; i++){
		if (i != s){
			close(i);
		}
	}
	printf("closed FDs...\n");

	FD_ZERO(&rfd);
	FD_SET(s, &rfd);
	
	while (1){
		c_rfd = rfd;
		printf("selecting...");
		rc = select(dsize, &c_rfd, NULL, NULL, (struct timeval *)NULL);
		printf("\n");

		if (FD_ISSET(s, &c_rfd)) {
			cs = accept(s, (struct sockaddr *)&csa, &size_csa);
			if (cs < 0){
				continue;
			}

			/* add the new socket to the set of open sockets */
			FD_SET(cs, &rfd);
			pClients->addClient(newSocket);
			pClients->printList();
			/* and loop again */
			continue;
		}

		for (i=3; i<dsize; i++){
			if (i != s && FD_ISSET(i, &c_rfd)){
				/* read from the socket */
				//rc = read(i, buf, BUFLEN);
				rc = recv(i,recvbuf,IRC_PACKET_SIZE,0);
				fprintf(stderr, "bytes_recv: %d\n",rc);
				/* if client closed the connection... */
				if (rc == 0) {
					/* close the socket */
					close(i);
					FD_CLR(i, &rfd);
				}else {/* if there was data to read */
					/* echo it back to the client */
					deserializeIRCPacket(&irc_msg,recvbuf);
					printf("%u,%u,%u\n%s\n",
							irc_msg.p.length,irc_msg.p.op_code,irc_msg.p.error_code,irc_msg.p.msg
					);
					switch(irc_msg.p.op_code){
						case 1:
						{
							ChannelNode *pChannelNode = (ChannelNode *)pChannels->searchName(irc_msg.p.msg);
							if(!pChannelNode){
								pChannels->addChannel(irc_msg.p.msg);
								//pChannels->getHead();
								pChannelNode=(ChannelNode *)pChannels->getHead();
							}else{
								printf("channel already exists!\n");
							}
							/*
							ClientNode *pClientNode = (ClientNode *)pClients->searchID(i);
							if(pClientNode){
								pChannelNode->addClient(pClientNode);
							}
							//*/
							if(pChannelNode)pChannelNode->printList();
							pChannels->printList();
						}
							break;
						case 2:

							break;
						default:
							break;
					}
					memset(&irc_msg,0,sizeof(irc_msg));
					serializeIRCPacket(sendbuf,&test);	
				//	deserializeIRCPacket(&des_test,sendbuf);	
				//	printf("%u\n%s\n",des_test.p.id,des_test.p.msg);
					int bytes_sent = 0;
					bytes_sent=send(i,sendbuf,IRC_PACKET_SIZE,0);
					
					if(bytes_sent < 0){
						fprintf(stderr, "error sending...");
						return;
					}
					fprintf(stderr, "bytes_sent: %d\n",bytes_sent);
				}
			}
		}
	}
}

void IRCServer::epoll_welcome(){

}	

void IRCServer::select_welcome(){
	int	i,rc,s,cs,dsize; 
	char buf[BUFLEN+1];  
	struct sockaddr_in sa; 
	struct sockaddr_in csa;
	socklen_t size_csa;
	fd_set rfd; 	
	fd_set c_rfd; 

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	sa.sin_addr.s_addr = INADDR_ANY;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0){
		perror("socket: allocation failed");
	}

	rc = bind(s, (struct sockaddr *)&sa, sizeof(sa));

	if(rc){
		perror("bind");
	}

	printf("listening...\n");
	rc = listen(s, 5);

	if(rc){
		perror("listen");
	}

	size_csa = sizeof(csa);
	dsize = getdtablesize();
	
	for (i=0; i<dsize; i++){
		if (i != s){
			close(i);
		}
	}
	printf("closed FDs...\n");

	FD_ZERO(&rfd);
	FD_SET(s, &rfd);
	
	while (1){
		c_rfd = rfd;
		printf("selecting...");
		rc = select(dsize, &c_rfd, NULL, NULL, (struct timeval *)NULL);
		printf("\n");

		if (FD_ISSET(s, &c_rfd)) {
			cs = accept(s, (struct sockaddr *)&csa, &size_csa);
			if (cs < 0){
				continue;
			}

			/* add the new socket to the set of open sockets */
			FD_SET(cs, &rfd);

			/* and loop again */
			continue;
		}

		for (i=0; i<dsize; i++){
			if (i != s && FD_ISSET(i, &c_rfd)){
				/* read from the socket */
				rc = read(i, buf, BUFLEN);
				/* if client closed the connection... */
				if (rc == 0) {
					/* close the socket */
					close(i);
					FD_CLR(i, &rfd);
				}else {/* if there was data to read */
					/* echo it back to the client */
				//	write(i, "hello server!", rc);
					write(i, "hello server!", BUFLEN);
				}
			}
		}
	}
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

	IRCPacket test = {42u,0u,0u,{"hello test packet!\0"}},
						des_test = {0,0,0,0};
	char sendbuf[IRC_PACKET_SIZE]={0};
//	printf("%u\n%s\n",test.p.id,test.p.msg);
	serializeIRCPacket(sendbuf,&test);	
	deserializeIRCPacket(&des_test,sendbuf);	
	//printf("%u\n%s\n",des_test.p.id,des_test.p.msg);
  /*---- send message to the socket of the incoming connection ----*/
	int bytes_sent = 0;
	bytes_sent=send(newSocket,sendbuf,IRC_PACKET_SIZE,0);
	
	if(bytes_sent < 0){
		fprintf(stderr, "error sending...");
		return;
	}
	fprintf(stderr, "bytes_sent: %d\n",bytes_sent);
}

