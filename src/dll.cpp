#include "dll.h"

DLL::DLL()
	:pHead(NULL),pTail(NULL),node_id(0),node_count(0){
}

DLL::~DLL(){
	fprintf(stderr,"~DLL() called!\n");
	if(pHead){
		fprintf(stderr,"destroying the list!\n");
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

//*
int DLL::removeNode(Node **pNode){
	if(!pNode){
		fprintf(stderr,"No **pNode to remove!\n");
		return EXIT_FAILURE;
	}
	if(pHead==pTail){
		pHead=pTail=NULL;
	}else if(*pNode==pHead){
		*pNode=NULL;
		return removeHead();
	}else if(*pNode==pTail){
		*pNode=NULL;
		return removeTail();
	}else{
		(*pNode)->getPrev()->setNext((*pNode)->getNext());
		(*pNode)->getNext()->setPrev((*pNode)->getPrev());
	}
	delete *pNode;
	*pNode=NULL;
	--node_count;
	return EXIT_SUCCESS;
}
//*/

int DLL::removeNode(Node *pNode){
	if(!pNode){
		fprintf(stderr,"No *pNode to remove!\n");
		return EXIT_FAILURE;
	}
	if(pHead==pTail){
		pHead=pTail=NULL;
	}else if(pNode==pHead){
		pNode=NULL;
		return removeHead();
	}else if(pNode==pTail){
		pNode=NULL;
		return removeTail();
	}else{
		pNode->getPrev()->setNext(pNode->getNext());
		pNode->getNext()->setPrev(pNode->getPrev());
	}
	delete pNode;
	pNode=NULL;
	--node_count;
	return EXIT_SUCCESS;
}

int DLL::removeHead(){
	if(!pHead){
		fprintf(stderr,"No head to remove!\n");
		return EXIT_FAILURE;
	}
	Node *pNode=pHead;
	if(!pHead->getNext()){
		pHead=NULL;
	}else{
		pHead=pHead->getNext();
		pHead->setPrev(NULL);
	}
	if(pNode)delete pNode;
	--node_count;
	return EXIT_SUCCESS;
}

int DLL::removeTail(){
	if(!pTail){
		fprintf(stderr,"No tail to remove!\n");
		return EXIT_FAILURE;
	}
	Node *pNode = pTail;
	if(!pTail->getPrev()){
			pTail=NULL;
	}else{
		pTail=pTail->getPrev();
		pTail->setNext(NULL);
	}
	if(pNode)delete pNode;
	--node_count;
	return EXIT_SUCCESS;
}

Node *DLL::searchID(unsigned int id){
	unsigned int tmp_id=0;
	Node *pTemp = NULL;
	if(!pHead){
		fprintf(stderr,"notlhing in list!!\n");
		return NULL;
	}
	pTemp=pHead;
	while(pTemp){
		tmp_id=pTemp->getNodeID();
		fprintf(stderr,"id: %d\n",tmp_id);
		if(tmp_id==id){
			fprintf(stderr,"id found!\n");
			return pTemp;
		}
		pTemp=pTemp->getNext();
	}
	fprintf(stderr,"id not found!\n");
	return NULL;
}

Node *DLL::searchName(char *name){
	Node *pTemp = NULL, *pExt = NULL;
	if(!pHead){
		return NULL;
	}
	pTemp=pHead;
	while(pTemp){
		pExt=pTemp->getExternal();
		if(pExt){
			if(strcmp(pExt->getName(),name)==0){
				fprintf(stderr,"%s found!\n",name);
				return pTemp;
			}
		}
		if(strcmp(pTemp->getName(),name)==0){
			fprintf(stderr,"%s found!\n",name);
			return pTemp;
		}
		pTemp=pTemp->getNext();
	}
	fprintf(stderr,"%s not found!\n",name);
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
	pHead=pTail=NULL;
	return i;
}
