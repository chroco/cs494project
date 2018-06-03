#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "irc.h"
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
		ClientNode(unsigned int id,ClientNode *pClientNode);
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
		void getList(char *list);
		int addClient(int socket);
		int addClient(ClientNode *pClientNode);
		ClientNode *searchSocket(int socket);
		ClientNode *createNode();
		ClientNode *createNode(int socket);
		ClientNode *createNode(ClientNode *pClientNode);
		~ClientList();
	private:
};

class ChannelNode:public Node{
	public:
		ChannelNode();
		ChannelNode(unsigned int id);
		ChannelNode(unsigned int id,char *n);
		void printList();
		void getList(char *list);
		int addClient(ClientNode *pClientNode);
		ClientNode *searchName(char *n);
		~ChannelNode();
	private:
		ClientList *pClients;
};

class ChannelList:public DLL{
	public:
		ChannelList();
		int addChannel(char *name);
		int removeChannel(char *name);
		ChannelNode *createNode();
		ChannelNode *createNode(char *name);
		void printList();
		void printList(char *n);
		void getList(char *list);
		~ChannelList();
	private:
};

#endif
