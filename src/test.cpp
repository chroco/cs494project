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

int Test::test_wordSerialization(){
	IRC *pIRC = new IRC();
	uint32_t number = 187;
	char word[4];
	printf("%u\n",number);
	pIRC->uint32_t_to_char4(word,number);
	number = 0;
	printf("%x %x %x %x\n", word[0], word[1], word[2], word[3]);
	pIRC->char4_to_uin32_t(&number,word);
	printf("%u\n",number);
	delete pIRC;
	if(number != 187){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Test::test_charArraySerialization(){
	IRC *pIRC = new IRC();
	char msg[MSG_SIZE] = {"Hello Serializer!\0"},
			 ser[MSG_SIZE] = {0},
			 res[MSG_SIZE] = {0};
	printf("msg: %s\n",msg);
	pIRC->serialize_msg(ser,msg);
	printf("ser: %s\n",ser);
	pIRC->deserialize_msg(res,ser);
	printf("res: %s\n",res);
	delete pIRC;
	if(strncmp(msg,res,MSG_SIZE)){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Test::test_packetSerialization(){
	IRC *pIRC = new IRC();
	IRCPacket msg = {42u,0,0,{"Hello Serializer!\0"}},
						res = {0,0,0,0};
	char buffer[IRC_PACKET_SIZE] = {0};	
	printf("calling Test::test_packetSerialization()\n");
	printf("msg: %u\n",msg.p.length);
	printf("msg: %s\n",msg.p.msg);
	pIRC->serialize_packet(buffer,msg.serial);
	printf("msg after serialization: %u\n",msg.p.length);
	printf("msg after serialization: %s\n",msg.p.msg);
	pIRC->deserialize_packet(res.serial,buffer);
	printf("msg after deserialization: %u\n",msg.p.length);
	printf("msg after deserialization: %s\n",msg.p.msg);
	fprintf(stderr,"res: %u\n",res.p.length);
	fprintf(stderr,"res: %s\n",res.p.msg);
	delete pIRC;
	if(msg.p.length != res.p.length){
		fprintf(stderr,"(%u,%u) id not equal!\n",msg.p.length,res.p.length);
		return EXIT_FAILURE;
	}
	if(strncmp(msg.p.msg,res.p.msg,PACKET_SIZE)){
		fprintf(stderr,"msg not equal!\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Test::test_serializeIRCPacket(){
	IRC *pIRC = new IRC();
	IRCPacket msg = {42u,0,0,{"Hello Serializer!\0"}},
						res = {0,0,0,0};
	char buffer[IRC_PACKET_SIZE] = {0};	
	printf("calling Test::test_serializeIRCPacket()\n");
	fprintf(stderr,"msg: %u\n",msg.p.length);
	fprintf(stderr,"msg: %s\n",msg.p.msg);
	pIRC->serializeIRCPacket(buffer,&msg);
	pIRC->deserializeIRCPacket(&res,buffer);
	fprintf(stderr,"res: %u\n",res.p.length);
	fprintf(stderr,"res: %s\n",res.p.msg);
	if(msg.p.length != res.p.length){
		fprintf(stderr,"(%u,%u) id not equal!\n",msg.p.length,res.p.length );
		return EXIT_FAILURE;
	}
	if(strncmp(msg.p.msg,res.p.msg,PACKET_SIZE)){
		fprintf(stderr,"msg not equal!\n");
		return EXIT_FAILURE;
	}delete pIRC;
	return EXIT_SUCCESS;
}

int Test::test_deserializeIRCPacket(){

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
	TEST_ASSERT_PTR_EQ(pDLL->searchID(4),pTestNode);
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
	TEST_ASSERT_INT_EQ(pChannels->removeNode(pChannels->getHead()),EXIT_SUCCESS);
	TEST_ASSERT_INT_EQ(pChannels->removeNode(pChannels->getTail()),EXIT_SUCCESS);
	TEST_ASSERT_INT_EQ(pChannels->destroyList(),2);
	TEST_ASSERT_INT_EQ(pChannels->getNodeCount(),0);
	TEST_ASSERT_INT_EQ(pChannels->removeNode(pChannels->getHead()),EXIT_FAILURE);
	TEST_ASSERT_INT_EQ(pChannels->removeNode(pChannels->getTail()),EXIT_FAILURE);
	delete pChannels;
	fprintf(stderr,"test_Channel_searchNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_Channel_searchName(){
	int i;
	char node_name[NAME_LENGTH]="node \0";
//*
	ChannelList *pChannels = new ChannelList();
	ChannelNode *pChannelNode=NULL;
	Node *pNode=NULL;

	for(i=0;i<5;++i){
		node_name[4] = '0' + i;
		pChannelNode = pChannels->createNode();
		if(i==4){
			pNode=dynamic_cast<Node *>(pChannelNode);
		}
		pChannels->insertNode(pChannelNode);
		pChannelNode = NULL;
	}
	TEST_ASSERT_INT_EQ(pChannels->getNodeCount(),i);
	node_name[4] = '0' + 4;
	pNode=pChannels->searchName(node_name);
	if(!pNode){
		fprintf(stderr,"pNode is NULL\n");
	}
	TEST_ASSERT_NOT_NULL(pNode);
	TEST_ASSERT_PTR_EQ(pChannels->searchName(node_name),pNode);
	TEST_ASSERT_INT_EQ(pChannels->removeNode(&pNode),EXIT_SUCCESS);
	TEST_ASSERT_PTR_EQ(pNode,NULL);
	TEST_ASSERT_INT_EQ(pChannels->destroyList(),4);
	TEST_ASSERT_INT_EQ(pChannels->getNodeCount(),0);
	delete pChannels;
//*/
	fprintf(stderr,"test_Channel_searchNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_Client_searchNode(){
	int i;
	ClientList *pClients = new ClientList();
	ClientNode *pClientNode=NULL;
	Node *pNode=NULL;

	for(i=0;i<5;++i){
		pClientNode = pClients->createNode();
		if(i==4){
			pNode=dynamic_cast<Node *>(pClientNode);
		}
		pClients->insertNode(pClientNode);
		pClientNode = NULL;
	}
	TEST_ASSERT_INT_EQ(pClients->getNodeCount(),i);
	pNode=pClients->searchID(4);
	if(!pNode){
		fprintf(stderr,"pNode is NULL\n");
	}
	TEST_ASSERT_NOT_NULL(pNode);
	TEST_ASSERT_PTR_EQ(pClients->searchID(4),pNode);
	TEST_ASSERT_INT_EQ(pClients->removeNode(&pNode),EXIT_SUCCESS);
	TEST_ASSERT_PTR_EQ(pNode,NULL);
	TEST_ASSERT_INT_EQ(pClients->removeNode(pClients->getHead()),EXIT_SUCCESS);
	TEST_ASSERT_INT_EQ(pClients->removeNode(pClients->getTail()),EXIT_SUCCESS);
	TEST_ASSERT_INT_EQ(pClients->destroyList(),2);
	TEST_ASSERT_INT_EQ(pClients->getNodeCount(),0);
	TEST_ASSERT_INT_EQ(pClients->removeNode(pClients->getHead()),EXIT_FAILURE);
	TEST_ASSERT_INT_EQ(pClients->removeNode(pClients->getTail()),EXIT_FAILURE);
	delete pClients;
	fprintf(stderr,"test_Channel_searchNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_Client_searchName(){
	int i;
	char node_name[NAME_LENGTH]="node \0";
//*
	ClientList *pClients = new ClientList();
	ClientNode *pClientNode=NULL;
	Node *pNode=NULL;

	for(i=0;i<5;++i){
		node_name[4] = '0' + i;
		pClientNode = pClients->createNode();
		if(i==4){
			pNode=dynamic_cast<Node *>(pClientNode);
		}
		pClients->insertNode(pClientNode);
		pClientNode = NULL;
	}
	TEST_ASSERT_INT_EQ(pClients->getNodeCount(),i);
	node_name[4] = '0' + 4;
	pNode=pClients->searchName(node_name);
	if(!pNode){
		fprintf(stderr,"pNode is NULL\n");
	}
	TEST_ASSERT_NOT_NULL(pNode);
	TEST_ASSERT_PTR_EQ(pClients->searchName(node_name),pNode);
	TEST_ASSERT_INT_EQ(pClients->removeNode(&pNode),EXIT_SUCCESS);
	TEST_ASSERT_PTR_EQ(pNode,NULL);
	TEST_ASSERT_INT_EQ(pClients->destroyList(),4);
	TEST_ASSERT_INT_EQ(pClients->getNodeCount(),0);
	delete pClients;
//*/
	fprintf(stderr,"test_Channel_searchNode() passed!\n");
	return EXIT_SUCCESS;
}

int Test::test_addClient(){
	ClientList *pClients = new ClientList();
	pClients->addClient(5);
	pClients->addClient(7);
	pClients->printList();
	
	delete pClients;
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

