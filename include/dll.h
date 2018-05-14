#ifndef _DLL_H_
#define _DLL_H_

#include <stdlib.h>

#include "node.h"
#include <stdio.h>
#include <string.h>

class DLL{
	public:
		DLL();
		virtual ~DLL();
	
		virtual Node *createNode();
		int insertNode(Node *pNode);
		int destroyList();
		Node *getHead();
		Node *getTail();
		long int getNodeCount();
		int removeNode(Node *pNode);
		int removeNode(Node **pNode);
		Node *searchID(unsigned int id);
		Node *searchName(char *);
		bool isEmptyList();
	protected:
		int insertHead(Node *pNode);
		int insertTail(Node *pNode);
		int removeHead();
		int removeTail();
		Node *pHead;
		Node *pTail;
		unsigned int node_id;
		long int node_count;
};


#endif 
