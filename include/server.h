#ifndef _SERVER_H_
#define _SERVER_H_

#include "irc.h"
#include "channel.h"

class IRCServer : public IRC{
	public:
		IRCServer();
		~IRCServer();
		void welcome();
		void respond();
		void helloSocket();
		void getStats();
	private:
		ChannelList *pChannels;
		int welcomeSocket, newSocket;
		struct sockaddr_storage serverStorage;
};

#endif
