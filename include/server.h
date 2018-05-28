#ifndef _SERVER_H_
#define _SERVER_H_

#include "irc.h"
#include "channel.h"



class IRCServer : public IRC{
	public:
		IRCServer();
		~IRCServer();
		void welcome();
		void getStats();
	private:
		ChannelList *pChannels;
		ClientList *pClients;
		int welcomeSocket, newSocket;
		struct sockaddr_storage serverStorage;
};

#endif
