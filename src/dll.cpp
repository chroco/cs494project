#include "dll.h"
DLL::DLL():pHead(NULL),pTail(NULL),node_id(0){
	
}

DLL::~DLL(){

}
	
int DLL::intsertHead(Node *pNode){
	if(pNode){
		return 1;
	}
	if(!pHead){
		pHead=pNode();
		pTail=pNode();
	}else{
		pNode->setNext(pHead);
		pHead->setPrev(pNode);
		pHead=pNode;
	}

	return 0;
}

int DLL::intsertTail(Node *pNode){

	return 0;
}

int DLL::removeHead(){
	if(!pHead){
		return 1;
	}
	Node *pNode=pHead;
	pHead=pHead->getNext();
	pHead->setPrev(NULL);
	delete pNode;

	return 0;
}

int DLL::removeTail(){

	return 0;
}

Node *DLL::search(unsigned int id){

	return NULL;
}

int DLL::destroyList(){

	return 0;
}
