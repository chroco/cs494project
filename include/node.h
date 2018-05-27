#ifndef _NODE_H_
#define _NODE_H_

#include <stdlib.h>
#include <cstddef>
#include <stdio.h>

#define NAME_LENGTH 32

class Node{
	public:
		Node();
		Node(unsigned int id);
		Node(unsigned int id,char *n);
		Node(unsigned int id,Node *ext);
		Node(Node *prev,Node *next,unsigned int id);
		virtual ~Node();

		Node *getPrev();
		Node *getNext();
		int setPrev(Node *pNode);
		int setNext(Node *pNode);
		unsigned int getNodeID();
		char *getName();
	protected:
		Node *pPrev;
		Node *pNext;
		Node *pExternal;
		unsigned int node_id;
		char name[NAME_LENGTH];
		
};

#endif
