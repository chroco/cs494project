#include "test.h"

Test::Test(){}

Test::~Test(){}

int Test::testNode(){
	Node *prev,*next,
			 *pNode = new Node();
	TEST_ASSERT(!pNode->getNext());
	TEST_ASSERT(!pNode->getPrev());
	TEST_ASSERT_INT_EQ(pNode->getNodeID(),0);
	fprintf(stderr,"testNode() passed!\n");
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
//*/
	fprintf(stderr,"testNode(prev,next,42) passed!\n");
	return 0;
}

int Test::testDLL(){

	return 0;
}

int Test::testIRC(){

	return 0;
}

int Test::testServer(){

	return 0;
}

int Test::testClient(){

	return 0;
}

