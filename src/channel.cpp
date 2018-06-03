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
	ClientNode *pTemp = NULL;
	if(!pHead){
		fprintf(stderr,"notlhing in list!!\n");
		return NULL;
	}
	pTemp=(ClientNode *)pHead;
	while(pTemp){
		tmp_socket=pTemp->getSocket();
//		fprintf(stderr,"socket: (%d,%d)\n",tmp_socket,socket);
		if(tmp_socket==socket){
//			fprintf(stderr,"socket found!\n");
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

void ClientList::getList(char *list){
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

ClientNode *ChannelNode::searchName(char *n){
	return (ClientNode *)pClients->searchName(n);
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
	int i=0,j=0;
	char *name=NULL;
	ChannelNode *pNode = (ChannelNode *)pHead;
	if(!pNode){
		printf("Channel list  empty!\n");
		return;
	}
	while(pNode){
		name=pNode->getName();
		for(;i<MSG_SIZE;++i){
			for(j=0;j<NAME_LENGTH && name[j] != '\0';++j){
				list[i]=name[j];	
			}
			list[i]=' ';
		}
		pNode=(ChannelNode *)pNode->getNext();
	}
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

ChannelNode *ChannelList::createNode(char *name){
	++node_count;
	return new ChannelNode(node_id,name);
}

