#include "channel.h"

// ****** Client code ******

ClientNode::ClientNode():Node(),client_socket(0){}

ClientNode::ClientNode(unsigned int id):Node(id),client_socket(0){}

ClientNode::ClientNode(unsigned int id,int socket)
	:Node(id),client_socket(socket){
	char sendbuf[5] = "ack\0";
	send(client_socket,sendbuf,5,0);
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
	ClientNode *pTemp = NULL;
	if(!pHead){
		fprintf(stderr,"notlhing in list!!\n");
		return NULL;
	}
	pTemp=(ClientNode *)pHead;
	while(pTemp){
		tmp_socket=pTemp->getSocket();
		fprintf(stderr,"socket: %d\n",tmp_socket);
		if(tmp_socket==socket){
			fprintf(stderr,"socket found!\n");
			return pTemp;
		}
		pTemp=(ClientNode *)pTemp->getNext();
	}
	fprintf(stderr,"id not found!\n");
	return NULL;
}

void ClientList::printList(){
	ClientNode *pNode = (ClientNode *)pHead;
	if(!pNode){
		printf("list is empty!\n");
		return;
	}
	while(pNode){
		printf("node_id: %u, socket: %d\n",pNode->getNodeID(),pNode->getSocket());
		pNode=(ClientNode *)pNode->getNext();
	}
}

int ClientList::addClient(int socket){
	return insertNode(createNode(socket));
}

int ClientList::addClient(ClientNode *pClientNode){
	return insertNode(createNode(pClientNode));
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

// ****** Channel code ******

ChannelNode::ChannelNode():Node(),pClients(NULL){
	pClients = new ClientList();
}

ChannelNode::ChannelNode(unsigned int id):Node(id){
	pClients = new ClientList();
}

ChannelNode::ChannelNode(unsigned int id,char *n):Node(id,n){
	pClients = new ClientList();
}

void ChannelNode::printList(){
	pClients->printList();
}

int ChannelNode::addClient(ClientNode *pClientNode){
	return pClients->addClient(pClientNode);
}

ChannelNode::~ChannelNode(){
	delete pClients;
}

ChannelList::ChannelList():DLL(){}

ChannelList::~ChannelList(){}

ChannelNode *ChannelList::createNode(){
	++node_count;
	return new ChannelNode(node_id);
}

void ChannelList::printList(){
	ChannelNode *pNode = (ChannelNode *)pHead;
	if(!pNode){
		printf("list is empty!\n");
		return;
	}
	while(pNode){
		printf("node_id: %u, name: %s\n",pNode->getNodeID(),pNode->getName());
		pNode=(ChannelNode *)pNode->getNext();
	}
}

int ChannelList::addChannel(char *name){
	return insertNode(createNode(name));
}

ChannelNode *ChannelList::createNode(char *name){
	++node_count;
	return new ChannelNode(node_id,name);
}

