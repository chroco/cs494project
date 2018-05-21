#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "node.h"
#include "dll.h"
#include <arpa/inet.h>
#include <sys/socket.h>

class ClientNode:public Node{
	public:
		ClientNode();
		ClientNode(unsigned int id);
		ClientNode(unsigned int id,int socket);
 		ClientNode(unsigned int id,char *n);
		int getSocket();
 		~ClientNode();
	private:
		int client_socket;
		struct sockaddr_storage serverStorage;
};

class ClientList:public DLL{
	public:
		ClientList();
		void printList();
		int addClient(int socket);
		ClientNode *createNode();
		ClientNode *createNode(int socket);
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
