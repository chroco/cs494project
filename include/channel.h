#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "node.h"
#include "dll.h"

class ClientNode:public Node{
	public:
		ClientNode();
		ClientNode(unsigned int id);
 		ClientNode(unsigned int id,char *n);
 		~ClientNode();
	private:
};

class ClientList:public DLL{
	public:
		ClientList();
		ClientNode *createNode();
		~ClientList();
	private:
};

class ChannelNode:public Node{
	public:
		ChannelNode();
		ChannelNode(unsigned int id);
		ChannelNode(unsigned int id,char *n);
		~ChannelNode();
	private:
		ClientList *pClients;
};

class ChannelList:public DLL{
	public:
		ChannelList();
		ChannelNode *createNode();
		~ChannelList();
	private:
};

#endif
