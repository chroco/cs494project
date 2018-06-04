#include "channel.h"

// ****** Client code ******

ClientNode::ClientNode():Node(),client_socket(0){}

ClientNode::ClientNode(unsigned int id):Node(id),client_socket(0){}

ClientNode::ClientNode(unsigned int id,int socket)
	:Node(id),client_socket(socket){
	char client[]="Client\0";
	setName(client);
//	char sendbuf[5] = "ack\0";
//	send(client_socket,sendbuf,5,0);
}

ClientNode::ClientNode(unsigned int id,char *n):Node(id,n){}

ClientNode::ClientNode(unsigned int id,ClientNode *pClientNode):Node(id,pClientNode){}

int ClientNode::getSocket(){
	return client_socket;
}	
	
ClientNode::~ClientNode(){}

ClientList::ClientList():DLL(){}

ClientList::~ClientList(){}

ClientNode *ClientList::searchSocket(int socket){
	int tmp_socket=0;
	ClientNode *pTemp = NULL,*pExt = NULL;
	if(!pHead){
		fprintf(stderr,"notlhing in list!!\n");
		return NULL;
	}
	pTemp=(ClientNode *)pHead;
	while(pTemp){
		pExt=(ClientNode *)pTemp->getExternal();
		if(pExt){
			tmp_socket=pExt->getSocket();
			if(tmp_socket==socket){
				fprintf(stderr,"socket found!\n");
				return pTemp;
			}
		}
		tmp_socket=pTemp->getSocket();
		if(tmp_socket==socket){
			fprintf(stderr,"socket found!\n");
			return pTemp;
		}
		pTemp=(ClientNode *)pTemp->getNext();
	}
	fprintf(stderr,"socket not found!\n");
	return NULL;
}

void ClientList::printList(){
	ClientNode *pNode = (ClientNode *)pHead,
						 *pExternal = NULL;
	if(!pNode){
		printf("ClientList is empty!\n");
		return;
	}
	while(pNode){
		pExternal=(ClientNode *)pNode->getExternal();
		if(pExternal){
			printf("clientNode_id: %u, socket: %d, name: %s\n",
					pExternal->getNodeID(),pExternal->getSocket(),pExternal->getName());
		}else{
			printf("clientNode_id: %u, socket: %d, name: %s\n",
					pNode->getNodeID(),pNode->getSocket(),pNode->getName());
		}
		pNode=(ClientNode *)pNode->getNext();
	}
}

void addName(Node *pNode,char *list,int &i){
	char *name=pNode->getName();
	if(name){
		printf("name: %s\n",name);
		for(int j=0;i<MSG_SIZE && j<NAME_LENGTH && name[j] != '\0';++j,++i){
			list[i]=name[j];	
		}
		list[i]=' ';
		list[++i]='\0';
	}
}

void ClientList::getList(char *list){
	int i=0;
	ClientNode *pNode = (ClientNode *)pHead,
						 *pExternal = NULL;
	if(!pNode){
		printf("ClientList is empty!\n");
		return;
	}
	while(pNode){
		pExternal=(ClientNode *)pNode->getExternal();
		if(pExternal){
			printf("clientNode_id: %u, socket: %d, name: %s\n",
					pExternal->getNodeID(),pExternal->getSocket(),pExternal->getName());
			addName((Node *)pExternal,list,i);
		}else{
			printf("clientNode_id: %u, socket: %d, name: %s\n",
					pNode->getNodeID(),pNode->getSocket(),pNode->getName());
			addName(pNode,list,i);
		}
		pNode=(ClientNode *)pNode->getNext();
	}
	printf("list: %s\n",list);
}

int ClientList::addClient(int socket){
	return insertNode(createNode(socket));
}

int ClientList::addClient(ClientNode *pClientNode){
	return insertNode(createNode(pClientNode));
}

void ClientList::msgClients(IRCPacket *pIRCPacket,int socket){
	IRCPacket reply = {0,0,0,0};
	IRC irc;
	ClientNode *pNode = (ClientNode *)pHead,
						 *pExternal = NULL;
	printf("msgClients: %s\n",pIRCPacket->p.msg);
	char sendbuf[IRC_PACKET_SIZE] = {'\0'};
	if(!pNode){
		printf("ClientList is empty!\n");
		char empty[]="ClientList is empty!\0";
		strncpy(reply.p.msg,empty,MSG_SIZE);
		irc.serializeIRCPacket(sendbuf,&reply);
		send(socket,sendbuf,IRC_PACKET_SIZE,0);
		return;
	}
	while(pNode){
		pExternal=(ClientNode *)pNode->getExternal();
		if(pExternal){
			printf("clientNode_id: %u, socket: %d, name: %s\n",
					pExternal->getNodeID(),pExternal->getSocket(),pExternal->getName());
			socket=pExternal->getSocket();
		}else{
			printf("clientNode_id: %u, socket: %d, name: %s\n",
					pNode->getNodeID(),pNode->getSocket(),pNode->getName());
			socket=pNode->getSocket();
		}
		printf("sending to client: %d,%s\n",pNode->getSocket(),pNode->getName());
		irc.serializeIRCPacket(sendbuf,pIRCPacket);
		send(socket,sendbuf,IRC_PACKET_SIZE,0);
		pNode=(ClientNode *)pNode->getNext();
	}
}

void ClientList::closeClients(){
	ClientNode *pNode = (ClientNode *)pHead,
						 *pExternal = NULL;
	int socket;
	if(!pNode){
		printf("ClientList is empty!\n");
		return;
	}
	while(pNode){
		pExternal=(ClientNode *)pNode->getExternal();
		if(!pExternal){
			socket=pNode->getSocket();
			printf("Closing socket: %d\n",socket);
			close(socket);
		}
		pNode=(ClientNode *)pNode->getNext();
	}
}

ClientNode *ClientList::createNode(){
	++node_count;
	return new ClientNode(node_id);
}

ClientNode *ClientList::createNode(int socket){
	++node_count;
	return new ClientNode(node_id,socket);
}

ClientNode *ClientList::createNode(ClientNode *pClientNode){
	++node_count;
	return new ClientNode(node_id,pClientNode);
}

// ****** ChannelNode ******

ChannelNode::ChannelNode():Node(){
	pClients = new ClientList();
}

ChannelNode::ChannelNode(unsigned int id):Node(id){
	pClients = new ClientList();
}

ChannelNode::ChannelNode(unsigned int id,char *n):Node(id,n){
	pClients = new ClientList();
}

void ChannelNode::printList(){
	if(pClients)pClients->printList();
}

void ChannelNode::getList(char *list){
	if(pClients)pClients->getList(list);
}

int ChannelNode::addClient(ClientNode *pClientNode){
	return pClients->addClient(pClientNode);
}

int ChannelNode::removeClient(ClientNode *pClientNode){
	return pClients->removeNode((ClientNode *)pClientNode);
}

int ChannelNode::getClientCount(){
	return pClients->getNodeCount(); 
}

ClientNode *ChannelNode::searchName(char *n){
	return (ClientNode *)pClients->searchName(n);
}

ClientNode *ChannelNode::searchSocket(int socket){
	return (ClientNode *)pClients->searchSocket(socket);
}

void ChannelNode::msgChannel(IRCPacket *pIRCPacket,int socket){
	fprintf(stderr,"msgChannel: %s\n",pIRCPacket->p.msg);
	if(pClients)pClients->msgClients(pIRCPacket,socket);
}

ChannelNode::~ChannelNode(){
	delete pClients;
}

// ********ChannelList*****************

ChannelList::ChannelList():DLL(){}

ChannelList::~ChannelList(){}

ChannelNode *ChannelList::createNode(){
	++node_count;
	return new ChannelNode(node_id);
}

void ChannelList::printList(char *n){
	ChannelNode *pNode = (ChannelNode *)pHead;
	if(!pNode){
		printf("list is empty!\n");
		return;
	}
	while(pNode){
		if(pNode->searchName(n)){
			printf("ChannelNode_id: %u, name: %s\n",pNode->getNodeID(),pNode->getName());
		}
		pNode=(ChannelNode *)pNode->getNext();
	}
}

void ChannelList::getList(char *n,char *list){
	int i=0;
	ChannelNode *pNode = (ChannelNode *)pHead;
	if(!pNode){
		printf("list is empty!\n");
		return;
	}
	while(pNode){
		if(pNode->searchName(n)){
			addName(pNode,list,i);
			printf("ChannelNode_id: %u, name: %s\n",pNode->getNodeID(),pNode->getName());
		}
		pNode=(ChannelNode *)pNode->getNext();
	}
	printf("list: %s\n",list);
}

void ChannelList::printList(){
	ChannelNode *pNode = (ChannelNode *)pHead;
	if(!pNode){
		printf("Channel list  empty!\n");
		return;
	}
	while(pNode){
		printf("ChannelNode_id: %u, name: %s\n",pNode->getNodeID(),pNode->getName());
		pNode=(ChannelNode *)pNode->getNext();
	}
}

void ChannelList::getList(char *list){
	int i=0;
	char empty[]="Channel list is empty!\0";
	ChannelNode *pNode = (ChannelNode *)pHead;
	if(!pNode){
		printf("Channel list  empty!\n");
		strncpy(list,empty,MSG_SIZE);	
		return;
	}
	while(pNode){
		addName((Node *)pNode,list,i);
		pNode=(ChannelNode *)pNode->getNext();
	}
	printf("list: %s\n",list);
}

int ChannelList::addChannel(char *name){
	return insertNode(createNode(name));
}

int ChannelList::removeChannel(char *name){
	Node *pNode = (Node *)searchName(name);
	if(!pNode){
		return 1;
	}
	removeNode(pNode);
	return 0;
}

int ChannelList::partChannel(char *name,int socket){
	ChannelNode *pChannelNode = (ChannelNode *)searchName(name);
	if(!pChannelNode){
		return 1;
	}
	//search for clients in the channel and remove them
	ClientNode *pClientNode = pChannelNode->searchSocket(socket);	
	if(pClientNode){
		if(pChannelNode->removeClient(pClientNode)){
			return 1;
		}
	}
	printf("Client Count: %d\n",pChannelNode->getClientCount());
	if(pChannelNode->getClientCount()==0){
		removeNode(pChannelNode);		
	}
	return 0;
}

int ChannelList::removeClient(ClientNode *pClientNode){
	ChannelNode *pChannelNode = (ChannelNode *)pHead;
	Node *pNode = NULL;
	if(!pChannelNode){
		return 1;
	}
	while(pChannelNode){
		pNode = pChannelNode->searchSocket(pClientNode->getSocket());	
		if(pNode){
			if(pChannelNode->removeClient((ClientNode *)pNode)){
				return 1;
			}
		}
		pChannelNode=(ChannelNode *)pChannelNode->getNext();
	}
//	pClients->removeNode((Node*)pClientNode);
	return 0;
}

ChannelNode *ChannelList::createNode(char *name){
	++node_count;
	return new ChannelNode(node_id,name);
}

