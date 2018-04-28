#ifndef _DLL_H_
#define _DLL_H_

#include "node.h"

class DLL{
	public:
		DLL();
		~DLL();
	
		int intsertHead(Node *pNode);
		int intsertTail(Node *pNode);
		int removeHead();
		int removeTail();
		Node *search(unsigned int id);
	protected:
		int destroyList();
		Node *pHead;
		Node *pTail;
		unsigned int node_id;
};


#endif 
