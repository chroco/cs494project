#include "test.h"

Test::Test(){}

Test::~Test(){}

int Test::test_Node(){
	Node *prev,*next,
			 *pNode = new Node();
	TEST_ASSERT(!pNode->getNext());
	TEST_ASSERT(!pNode->getPrev());
	TEST_ASSERT_INT_EQ(pNode->getNodeID(),0);
	delete pNode;
//*
	prev = new Node();
	next = new Node();
	pNode = new Node(prev,next,42);
	TEST_ASSERT_PTR_EQ(pNode->getPrev(),prev);
	TEST_ASSERT_PTR_EQ(pNode->getNext(),next);
	TEST_ASSERT_INT_EQ(pNode->getNodeID(),42);
	delete pNode;
	delete next;
	delete prev;
	pNode = new Node(42);
	TEST_ASSERT_INT_EQ(pNode->getNodeID(),42);
	delete pNode;
//*/
	fprintf(stderr,"test_Node() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_DLL_createNode(){
	DLL *pDLL = new DLL();
	Node *pNode = pDLL->createNode();
	TEST_ASSERT(pNode);
	delete pNode;
	delete pDLL;
	fprintf(stderr,"test_createNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_DLL_insertNode(){
	DLL *pDLL = new DLL();
	Node *pNode = pDLL->createNode();
	pDLL->insertNode(pNode);
	TEST_ASSERT_PTR_EQ(pDLL->getHead(),pNode);
//	delete pNode; **freed already
	delete pDLL;
	fprintf(stderr,"test_insertNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_DLL_searchNode(){
	int i;
	DLL *pDLL = new DLL();
	Node *pNode=NULL,
			 *pTestNode=NULL;

	for(i=0;i<5;++i){
		pNode = pDLL->createNode();
		if(i==4){
			pTestNode=pNode;
		}
		pDLL->insertNode(pNode);
		pNode=NULL;
	}
	TEST_ASSERT_INT_EQ(pDLL->getNodeCount(),i);
	pNode=pDLL->searchID(4);
	if(!pNode){
		fprintf(stderr,"pNode is NULL\n");
	}
	TEST_ASSERT_NOT_NULL(pNode);
	TEST_ASSERT_PTR_EQ(pDLL->searchID(4),pNode);
	TEST_ASSERT_INT_EQ(pDLL->removeNode(&pNode),EXIT_SUCCESS);
	TEST_ASSERT_PTR_EQ(pNode,NULL);
	TEST_ASSERT_INT_EQ(pDLL->destroyList(),4);
	TEST_ASSERT_INT_EQ(pDLL->getNodeCount(),0);
	if(pNode)delete pNode;
	delete pDLL;
	fprintf(stderr,"test_DLL_searchNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_Channel_searchNode(){
	int i;
	ChannelList *pChannels = new ChannelList();
	ChannelNode *pChannelNode=NULL;
	Node *pNode=NULL;

	for(i=0;i<5;++i){
		pChannelNode = pChannels->createNode();
		if(i==4){
			pNode=dynamic_cast<Node *>(pChannelNode);
		}
		pChannels->insertNode(pChannelNode);
		pChannelNode = NULL;
	}
	TEST_ASSERT_INT_EQ(pChannels->getNodeCount(),i);
	pNode=pChannels->searchID(4);
	if(!pNode){
		fprintf(stderr,"pNode is NULL\n");
	}
	TEST_ASSERT_NOT_NULL(pNode);
	TEST_ASSERT_PTR_EQ(pChannels->searchID(4),pNode);
	TEST_ASSERT_INT_EQ(pChannels->removeNode(&pNode),EXIT_SUCCESS);
	TEST_ASSERT_PTR_EQ(pNode,NULL);
	TEST_ASSERT_INT_EQ(pChannels->destroyList(),4);
	TEST_ASSERT_INT_EQ(pChannels->getNodeCount(),0);
	delete pChannels;
	fprintf(stderr,"test_Channel_searchNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_Channel_searchName(){
	int i;
/*
	ChannelList *pChannels = new ChannelList();
	ChannelNode *pChannelNode=NULL;
	Node *pNode=NULL;

	for(i=0;i<5;++i){
		pChannelNode = pChannels->createNode();
		if(i==4){
			pNode=dynamic_cast<Node *>(pChannelNode);
		}
		pChannels->insertNode(pChannelNode);
		pChannelNode = NULL;
	}
	TEST_ASSERT_INT_EQ(pChannels->getNodeCount(),i);
	pNode=pChannels->searchNode(4);
	if(!pNode){
		fprintf(stderr,"pNode is NULL\n");
	}
	TEST_ASSERT_NOT_NULL(pNode);
	TEST_ASSERT_PTR_EQ(pChannels->searchNode(4),pNode);
	TEST_ASSERT_INT_EQ(pChannels->removeNode(&pNode),EXIT_SUCCESS);
	TEST_ASSERT_PTR_EQ(pNode,NULL);
	TEST_ASSERT_INT_EQ(pChannels->destroyList(),4);
	TEST_ASSERT_INT_EQ(pChannels->getNodeCount(),0);
	delete pChannels;
//*/
	fprintf(stderr,"test_Channel_searchNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_DLL(){

	return EXIT_SUCCESS;
}

int Test::test_IRC(){

	return EXIT_SUCCESS;
}

int Test::test_Server(){

	return EXIT_SUCCESS;
}

int Test::test_Client(){

	return EXIT_SUCCESS;
}

