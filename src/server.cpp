#include "server.h"

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
						irc_msg = {0,0,0,0};//,
//						des_test = {0,0,0,0};
	char sendbuf[IRC_PACKET_SIZE]={0},
			 recvbuf[IRC_PACKET_SIZE]={0};
	
	int	i,rc,s,cs,dsize; 
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
	
	for (i=3; i<dsize; ++i){
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

			FD_SET(cs, &rfd);
			pClients->addClient(cs);
			pClients->printList();
			continue;
		}
		for (i=3; i<dsize; ++i){
			if (i != s && FD_ISSET(i, &c_rfd)){
				rc = recv(i,recvbuf,IRC_PACKET_SIZE,0);
				fprintf(stderr, "bytes_recv: %d\n",rc);
				if (rc == 0) {
					fprintf(stderr, "closing the socket\n");
					close(i);
					FD_CLR(i, &rfd);
				}else { 
					deserializeIRCPacket(&irc_msg,recvbuf);
					printf("%u,%u,%u\n%s\n",
							irc_msg.p.length,irc_msg.p.op_code,irc_msg.p.error_code,irc_msg.p.msg
					);
					switch(irc_msg.p.op_code){
						case JOIN:
						{
							ChannelNode *pChannelNode = (ChannelNode *)pChannels->searchName(irc_msg.p.msg);
							if(!pChannelNode){
								pChannels->addChannel(irc_msg.p.msg);
								pChannelNode=(ChannelNode *)pChannels->getHead();
							}else{
								printf("channel %s already exists!\n",pChannelNode->getName());
							}
							//*
							ClientNode *pClientNode = (ClientNode *)pClients->searchSocket(i);
							if(pClientNode){
								printf("found client, adding to channel!\n");
								pChannelNode->addClient(pClientNode);
							}
							//*/
							if(pChannelNode)pChannelNode->printList();
							pChannels->printList();
						} break;
						case PART:
							pChannels->removeChannel(irc_msg.p.msg);
							pChannels->printList();
							break;
						default:
							break;
					}
					memset(&irc_msg,0,sizeof(irc_msg));
					serializeIRCPacket(sendbuf,&test);	
					int bytes_sent=send(i,sendbuf,IRC_PACKET_SIZE,0);
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
void IRCServer::getStats(){
	printf("sizeof(IRCPacket): %zu\n",sizeof(IRCPacket));
	printf("sizeof(buffer): %zu\n",sizeof(buffer));
	printf("sizeof(char)*PACKET_SIZE: %zu\n",sizeof(char)*PACKET_SIZE);
}

