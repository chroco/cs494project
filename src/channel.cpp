#include "channel.h"

ChannelNode::ChannelNode():Node(),pChannelName(NULL){

}

ChannelNode::ChannelNode(unsigned int id):Node(id),pChannelName(NULL){
	
}

ChannelNode::~ChannelNode(){

}

char *ChannelNode::getChannelName(){
	return pChannelName;
}

ChannelList::ChannelList(){
	DLL();
}

ChannelList::~ChannelList(){

}

ChannelNode *ChannelList::createNode(){
	++node_count;
	return new ChannelNode(node_id);
}

ChannelNode *ChannelList::searchChannel(char *id){
	ChannelNode *pTemp = NULL;
	if(!pHead){
		return NULL;
	}
	pTemp=(ChannelNode*)pHead;
	while(pTemp){
		if(strcmp(pTemp->getChannelName(),id)){
			return pTemp;
		}
		pTemp=(ChannelNode *)pTemp->getNext();
	}
	return NULL;
}
