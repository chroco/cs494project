#include "channel.h"

// ****** Client code ******

ClientNode::ClientNode():Node(){}

ClientNode::ClientNode(unsigned int id):Node(id){}

ClientNode::ClientNode(unsigned int id,char *n):Node(id,n){}

ClientNode::~ClientNode(){}

ClientList::ClientList():DLL(){}

ClientList::~ClientList(){}

ClientNode *ClientList::createNode(){
	++node_count;
	return new ClientNode(node_id);
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


