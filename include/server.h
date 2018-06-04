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
		int sendPacket(IRCPacket *pIRCPacket,int socket);
		void consoleInput();
	private:
		int handlePacket(IRCPacket *pIRCPacket,int socket);
		int joinChannel(IRCPacket *pIRCPacket,int socket);
		int partChannel(IRCPacket *pIRCPacket,int socket);
		int listThings(IRCPacket *pIRCPacket,int socket);
		int changeNick(IRCPacket *pIRCPacket,int socket);
		int msg(IRCPacket *pIRCPacket,int socket);
		int msgClient(IRCPacket *pIRCPacket,int socket);
		int msgChannel(IRCPacket *pIRCPacket,int socket);
		int clientExit(IRCPacket *pIRCPacket,int socket);
		ChannelList *pChannels;
		ClientList *pClients;
		int welcomeSocket, newSocket;
		struct sockaddr_storage serverStorage;
		pthread_t recv_thread;
};

#endif
