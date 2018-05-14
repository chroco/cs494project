#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "node.h"
#include "dll.h"

class ChannelNode : public Node{
	public:
		ChannelNode();
		ChannelNode(unsigned int id);
		~ChannelNode();
	private:
};

class ChannelList : public DLL{
	public:
		ChannelList();
		~ChannelList();
		ChannelNode *createNode();
		ChannelNode *searchChannel(char *channel_name);

	private:

};

#endif
