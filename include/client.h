#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "irc.h"

class IRCClient : public IRC{
	public:
		IRCClient();
		~IRCClient();
		void helloSocket();
		void requestConnection();
		int base_joinServer();
		int joinServer();
	private:
		int clientSocket;
};

#endif
