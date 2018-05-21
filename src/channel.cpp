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

int ClientNode::getSocket(){
	return client_socket;
}	
	
ClientNode::~ClientNode(){}

ClientList::ClientList():DLL(){}

ClientList::~ClientList(){}

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

ClientNode *ClientList::createNode(){
	++node_count;
	return new ClientNode(node_id);
}

ClientNode *ClientList::createNode(int socket){
	++node_count;
	return new ClientNode(node_id,socket);
}

// ****** Channel code ******

ChannelNode::ChannelNode():Node(),pClients(NULL){
	pClients = new ClientList();
}

ChannelNode::ChannelNode(unsigned int id):Node(id){}

ChannelNode::ChannelNode(unsigned int id,char *n):Node(id,n){}

ChannelNode::~ChannelNode(){
	delete pClients;
}

ChannelList::ChannelList():DLL(){}\

ChannelList::~ChannelList(){}

ChannelNode *ChannelList::createNode(){
	++node_count;
	return new ChannelNode(node_id);
}


