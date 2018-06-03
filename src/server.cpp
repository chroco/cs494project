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
	IRCPacket irc_msg = {0,0,0,0};
	char recvbuf[IRC_PACKET_SIZE]={0};
	
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
			continue;
		}
		for (i=3; i<dsize; ++i){
			if (i != s && FD_ISSET(i, &c_rfd)){
				rc = recv(i,recvbuf,IRC_PACKET_SIZE,0);
				fprintf(stderr, "bytes_recv: %d\n",rc);
				if (rc == 0) {
					fprintf(stderr, "closing socket %d\n",i);
					close(i);
					FD_CLR(i, &rfd);
					Node *pNode=(Node *)pClients->searchSocket(i);
					if (pNode){
						pClients->removeNode(pNode);
					}
				}else { 
					deserializeIRCPacket(&irc_msg,recvbuf);
					printf("%u,%u,%u\n%s\n\n",
							irc_msg.p.length,irc_msg.p.op_code,irc_msg.p.error_code,irc_msg.p.msg
					);
					handlePacket(&irc_msg,i);
					memset(&irc_msg,0,sizeof(irc_msg));
				}
			}
		}
	}
}

int IRCServer::sendPacket(IRCPacket *pIRCPacket,int socket){
	char sendbuf[IRC_PACKET_SIZE] = {0};
	int bytes_sent;
	serializeIRCPacket(sendbuf,pIRCPacket);	
	bytes_sent=send(socket,sendbuf,IRC_PACKET_SIZE,0);
	if(bytes_sent < 0){
		fprintf(stderr, "error sending...");
		return 1;
	}
	fprintf(stderr, "bytes_sent: %d\n",bytes_sent);
	return 0;
}

int IRCServer::joinChannel(IRCPacket *pIRCPacket,int socket){
	ChannelNode *pChannelNode;
	IRCPacket reply = {0,0,0,"Added to channel!\0"};
	if(pIRCPacket->p.msg[0]!='#'){
		printf("/join %s rejected!\n",pIRCPacket->p.msg);
		return 1;
	}
	pChannelNode = (ChannelNode *)pChannels->searchName(pIRCPacket->p.msg);
	if(!pChannelNode){
		pChannels->addChannel(pIRCPacket->p.msg);
		pChannelNode=(ChannelNode *)pChannels->getHead();
	}else{
		printf("channel %s already exists!\n",pChannelNode->getName());
	}
	ClientNode *pClientNode = (ClientNode *)pClients->searchSocket(socket);
	if(pClientNode){
		printf("Adding %s to channel %s!\n",pClientNode->getName(),pIRCPacket->p.msg);
		pChannelNode->addClient(pClientNode);
		sendPacket(&reply,pClientNode->getSocket());
	}
	return 0;
}

int IRCServer::partChannel(IRCPacket *pIRCPacket,int socket){
	if(pIRCPacket->p.msg[0]!='#'){
		return 1;
	}
	return pChannels->removeChannel(pIRCPacket->p.msg);
}

int IRCServer::listThings(IRCPacket *pIRCPacket,int socket){
	IRCPacket reply={0,0,0,"List default reply fix me!\0"};
	char list[MSG_SIZE]={'\0'};
	if(pIRCPacket->p.msg[0]=='#'){
		ChannelNode *pChannelNode;
		pChannelNode = (ChannelNode *)pChannels->searchName(pIRCPacket->p.msg);
		if(pChannelNode){
			pChannelNode->printList();
			pChannelNode->getList(list);
			strncpy(reply.p.msg,list,MSG_SIZE);
		}
	}else{
		if(!pIRCPacket->p.msg[0]){
			pChannels->printList();
			pChannels->getList(list);
			strncpy(reply.p.msg,list,MSG_SIZE);
		}else if(pClients->searchName(pIRCPacket->p.msg)){
			printf("Printing channels containing %s\n",pIRCPacket->p.msg);
			pChannels->printList(pIRCPacket->p.msg);
			pChannels->getList(pIRCPacket->p.msg,list);
			strncpy(reply.p.msg,list,MSG_SIZE);
		}
	}
	return sendPacket(&reply,socket);
}

int IRCServer::changeNick(IRCPacket *pIRCPacket,int socket){
	IRCPacket reply={0,0,0,0};
	char success[]="Nickname successfully changed!\0",
			 failure[]="Failed to change nickname";
	if(pIRCPacket->p.msg[0]!='#'){
		ClientNode *pClientNode = (ClientNode *)pClients->searchSocket(socket);
		if(pClientNode && pIRCPacket->p.msg && pIRCPacket->p.msg[0]!='\0'){
			pClientNode->setName(pIRCPacket->p.msg);
			strcpy(reply.p.msg,success);	
			sendPacket(&reply,pClientNode->getSocket());
		}else{
			strcpy(reply.p.msg,failure);	
			sendPacket(&reply,pClientNode->getSocket());
		}
	}else{
		fprintf(stderr,"Illegal Nickname: %s\n",pIRCPacket->p.msg);
		return 1;
	}
	return 0;
}

int IRCServer::msgClient(IRCPacket *pIRCPacket,int socket){
	char name_buf[NAME_LENGTH] = {0},
			 sendbuf[IRC_PACKET_SIZE] = {0};
	ClientNode *pClientNode = NULL;
	if(pIRCPacket->p.msg[0]=='#'){
		fprintf(stderr,"Invalid Nickname: %s\n",pIRCPacket->p.msg);
		return 1;
	}

	truncateFirstWord(name_buf,pIRCPacket->p.msg,NAME_LENGTH);
	pClientNode = (ClientNode *)pClients->searchName(name_buf);
	if(pClientNode){
		printf("messaging %s: %s\n",name_buf,pIRCPacket->p.msg);
		serializeIRCPacket(sendbuf,pIRCPacket);	
		int bytes_sent=send(pClientNode->getSocket(),sendbuf,IRC_PACKET_SIZE,0);
		if(bytes_sent < 0){
			fprintf(stderr, "error sending...");
			return 1;
		}
		fprintf(stderr, "bytes_sent: %d\n",bytes_sent);
	}
	
	return 0;
}

int IRCServer::handlePacket(IRCPacket *pIRCPacket,int socket){
	switch(pIRCPacket->p.op_code){
		case JOIN:
			return joinChannel(pIRCPacket,socket);	
		case PART:
			return partChannel(pIRCPacket,socket);
		case LIST:
			return listThings(pIRCPacket,socket);
		case NICK:
			return changeNick(pIRCPacket,socket);
		case MSG:
			return msgClient(pIRCPacket,socket);
		default:
			break;
	}
	return 0;
}

void IRCServer::getStats(){
	printf("sizeof(IRCPacket): %zu\n",sizeof(IRCPacket));
	printf("sizeof(buffer): %zu\n",sizeof(buffer));
	printf("sizeof(char)*PACKET_SIZE: %zu\n",sizeof(char)*PACKET_SIZE);
}

