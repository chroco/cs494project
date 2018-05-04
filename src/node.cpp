#include "node.h"

Node::Node()
	: pPrev(NULL),pNext(NULL),node_id(0){

}

Node::Node(Node *prev,Node *next,unsigned int id)
	: pPrev(prev),pNext(next),node_id(id){

}

Node::~Node(){
	pPrev=pNext=NULL;
/*
	if(pNext){
		delete pNext;
	}
	if(pPrev){
		delete pPrev;
	}
//*/
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
