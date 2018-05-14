#include "node.h"

Node::Node()
	: pPrev(NULL),pNext(NULL),node_id(0),pNodeName(NULL){
}

Node::Node(unsigned int id)
	: pPrev(NULL),pNext(NULL),node_id(id),pNodeName(NULL){
}

Node::Node(signed int id)
	: pPrev(NULL),pNext(NULL),node_id(id),pNodeName(NULL){
}

Node::Node(Node *prev,Node *next,unsigned int id)
	: pPrev(prev),pNext(next),node_id(id),pNodeName(NULL){
}

Node::~Node(){
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
	return pNodeName;
}

