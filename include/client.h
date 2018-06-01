#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "irc.h"

class IRCClient : public IRC{
	public:
		IRCClient();
		~IRCClient();
		void *recvMessage(void *ptr);
		int joinServer();
	private:
		int clientSocket;
};

#endif
