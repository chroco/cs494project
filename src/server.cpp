#include "server.h"
#include <pthread.h>

IRCServer::IRCServer():pChannels(NULL),welcomeSocket(0),newSocket(0),recv_thread(0){
	memset(&serverStorage, 0, sizeof(serverStorage));
	pChannels = new ChannelList();
	pClients = new ClientList();
}

IRCServer::~IRCServer(){
	delete pChannels;
	delete pClients;
	pthread_cancel(recv_thread);
}

void IRCServer::consoleInput(){
	char eom='\0',
		 message[BUFLEN]={'\0'};
	while(1){
		printf("\nType things o_0: ");
		scanf("%2000[^\n]%c", message,&eom);
		eom='\0';
		fprintf(stderr,"input: %s\n",message);
		if(strncmp(message,"/exit\0",BUFLEN)==0){
			fprintf(stderr,"Closing clients\n");
			if(pClients){
				pClients->closeClients();
			}
			delete this;
			exit(0);
		}
		memset(&message,0,sizeof(message));
	}
}

//IRCServer *pIRCServer;

void *recvMessageWrapper(void *ptr){
	IRCServer *pIRCServer=(IRCServer *)ptr;
	pIRCServer->consoleInput();
	return NULL;
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

	if(pthread_create(&recv_thread,NULL,recvMessageWrapper,this)){
		fprintf(stderr,"Cannot create thread...\n");
		close(rc);
		return;
	}

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
					if(pNode){
						pChannels->removeClient((ClientNode *)pNode);
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
	IRCPacket reply={0,0,0,"Parted channel!\0"};
	char failure[]="Failed to part channel!\0";
	if(pIRCPacket->p.msg[0]!='#'){
		strncpy(reply.p.msg,failure,MSG_SIZE);
		sendPacket(&reply,socket);
		return 1;
	}
	if(pChannels->partChannel(pIRCPacket->p.msg,socket)){
		strncpy(pIRCPacket->p.msg,failure,MSG_SIZE);
	}

	return sendPacket(&reply,socket);
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
			sendPacket(&reply,socket);
		}else{
			strcpy(reply.p.msg,failure);	
			sendPacket(&reply,socket);
		}
	}else{
		fprintf(stderr,"Illegal Nickname: %s\n",pIRCPacket->p.msg);
		strcpy(reply.p.msg,failure);	
		sendPacket(&reply,socket);
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

/*
int IRCServer::msgChannel(IRCPacket *pIRCPacket,int socket){
	char name_buf[NAME_LENGTH] = {0};
	ChannelNode *pChannelNode = NULL;
	
	truncateFirstWord(name_buf,pIRCPacket->p.msg,NAME_LENGTH);
	if(name_buf[0]!='#'){
		fprintf(stderr,"Invalid Channel: %s\n",name_buf);
		return 1;
	}
	pChannelNode=(ChannelNode *)pChannels->searchName(name_buf);
	if(pChannelNode){
		pChannelNode->msgChannel(pIRCPacket,socket);
		printf("messaging channel: %s\n",name_buf);
	}
	printf("message: %s\n",pIRCPacket->p.msg);
	return 0;
}
//*/

int IRCServer::msgChannel(IRCPacket *pIRCPacket,int socket){
	char name_buf[NAME_LENGTH] = {'\0'},
			 *channel[MAX_CHANNELS] = {NULL};
	ChannelNode *pChannelNode = NULL;
	int i=0;

	for(;i<MAX_CHANNELS && pIRCPacket->p.msg[0]=='#';++i){
		memset(name_buf,0,NAME_LENGTH);
		truncateFirstWord(name_buf,pIRCPacket->p.msg,NAME_LENGTH);
		channel[i] = new char[NAME_LENGTH];
		strncpy(channel[i],name_buf,NAME_LENGTH);
	}
	for(i=0;channel[i] && i<MAX_CHANNELS;++i){
		pChannelNode=(ChannelNode *)pChannels->searchName(channel[i]);
		if(pChannelNode){
			pChannelNode->msgChannel(pIRCPacket,socket);
		}
	}
	return 0;
}

int IRCServer::msg(IRCPacket *pIRCPacket,int socket){
	if(pIRCPacket->p.msg[0]=='#'){
		return msgChannel(pIRCPacket,socket);
	}
	return msgClient(pIRCPacket,socket);
}

int IRCServer::clientExit(IRCPacket *pIRCPacket,int socket){
	ClientNode *pClientNode = (ClientNode *)pClients->searchSocket(socket);
	if(pClientNode){
		pChannels->removeClient(pClientNode);
		pClients->removeNode(pClientNode);
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
			return msg(pIRCPacket,socket);
		case EXIT:
			return clientExit(pIRCPacket,socket);
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

