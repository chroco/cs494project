#ifndef _NODE_H_
#define _NODE_H_

#include <cstddef>

class Node{
	public:
		Node();
		Node(unsigned int id);
		Node(Node *prev,Node *next,unsigned int id);
		~Node();

		Node *getPrev();
		Node *getNext();
		int setPrev(Node *pNode);
		int setNext(Node *pNode);
		unsigned int getNodeID();
	protected:
		Node *pPrev;
		Node *pNext;
		unsigned int node_id;
		
};

#endif
