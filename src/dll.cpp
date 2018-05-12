#include "dll.h"

DLL::DLL()
	:pHead(NULL),pTail(NULL),node_id(0),node_count(0){
}

DLL::~DLL(){
	if(pHead){
		destroyList();
	}
}

Node *DLL::createNode(){
	++node_count;
	return new Node(node_id);
}

int DLL::insertNode(Node *pNode){
	return insertHead(pNode);
}

Node *DLL::getHead(){
	return pHead;
}

Node *DLL::getTail(){
	return pTail;
}

long int DLL::getNodeCount(){
	return node_count;
}

int DLL::insertHead(Node *pNode){
	if(!pNode){
		return EXIT_FAILURE;
	}
	if(!pHead){
		pHead=pTail=pNode;
	}else{
		pNode->setNext(pHead);
		pHead->setPrev(pNode);
		pHead=pNode;
	}
	++node_id;
	return EXIT_SUCCESS;
}

int DLL::insertTail(Node *pNode){

	return EXIT_SUCCESS;
}

int DLL::removeNode(Node **pNode){
	if(!(*pNode)){
		return EXIT_FAILURE;
	}
	if(pHead==pTail){
		pHead=pTail=NULL;
		delete *pNode;
	}else if(*pNode==pHead){
		*pNode=NULL;
		return removeHead();
	}else if(*pNode==pTail){
		*pNode=NULL;
		return removeTail();
	}else{
		(*pNode)->getPrev()->setNext((*pNode)->getNext());
		(*pNode)->getNext()->setPrev((*pNode)->getPrev());
		delete pNode;
	}
	*pNode=NULL;
	--node_count;
	return EXIT_SUCCESS;
}

int DLL::removeHead(){
	if(!pHead){
		return EXIT_FAILURE;
	}
	Node *pNode=pHead;
	if(!pHead->getNext()){
		pHead=NULL;
	}else{
		pHead=pHead->getNext();
		pHead->setPrev(NULL);
	}
	delete pNode;
	--node_count;
	return EXIT_SUCCESS;
}

int DLL::removeTail(){
	if(!pTail){
		return EXIT_FAILURE;
	}
	Node *pNode = pTail;
	pTail=pNode->getPrev();
	pTail->setNext(NULL);
	delete pNode;
	--node_count;
	return EXIT_SUCCESS;
}

Node *DLL::searchNode(unsigned int id){
	Node *pTemp = NULL;
	if(!pHead){
		return NULL;
	}
	pTemp=pHead;
	while(pTemp){
		if(pTemp->getNodeID()==id){
			return pTemp;
		}
		pTemp=pTemp->getNext();
	}
	return NULL;
}

bool DLL::isEmptyList(){
	return (!pHead)?true:false;
}

int DLL::destroyList(){
	int i=0;
	for(;pHead;++i){
		if(removeHead()){
			fprintf(stderr,"Error removing pHead!\n");
		}
	}
	return i;
}
