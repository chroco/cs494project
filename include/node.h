#ifndef _NODE_H_
#define _NODE_H_

#include <cstddef>

class Node{
	public:
		Node();
		Node(Node *prev,Node *next,unsigned int id);
		~Node();

		Node *getPrev();
		Node *getNext();
	protected:
		Node *pPrev;
		Node *pNext;
		unsigned int node_id;
		
};

#endif
