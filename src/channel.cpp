#include "channel.h"

ChannelNode::ChannelNode():Node(){

}

ChannelNode::ChannelNode(unsigned int id):Node(id){
	
}

ChannelNode::~ChannelNode(){

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


