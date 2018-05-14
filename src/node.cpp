#include "node.h"

Node::Node()
	: pPrev(NULL),pNext(NULL),node_id(0),name("\0"){

}

Node::Node(unsigned int id)
	: pPrev(NULL),pNext(NULL),node_id(id),name("\0"){
}

Node::Node(unsigned int id,char *n)
	: pPrev(NULL),pNext(NULL),node_id(id),name("\0"){
	int i = 0;
	for(;i < NAME_LENGTH && n[i] != '\0';++i){
		name[i] = n[i];	
	}
}

Node::Node(Node *prev,Node *next,unsigned int id)
	: pPrev(prev),pNext(next),node_id(id),name("\0"){
}

Node::~Node(){
	fprintf(stderr,"destroyed node id: %d!\n",node_id);
	pPrev=pNext=NULL;
}

Node *Node::getNext(){
	return pNext;
}

Node *Node::getPrev(){
	return pPrev;
}

int Node::setNext(Node *pNode){
	pNext=pNode;
	return 0;
}

int Node::setPrev(Node *pNode){
	pPrev=pNode;
	return 0; 
}

unsigned int Node::getNodeID(){
	return node_id;
}

char *Node::getName(){
	return name;
}

