#include "client.h"

IRCClient::IRCClient():clientSocket(0){}

IRCClient::~IRCClient(){}

void IRCClient::helloSocket(){
  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(PORT);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr(HOME);
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  /*---- Read the message from the server into the buffer ----*/
	IRCPacket recv_buf,test;
	uint32_t uint32_t_buf=0,testint=1;
	char rcvbuf[MSG_SIZE]={0};
	char msgbuf[MSG_SIZE]={0};
	recv(clientSocket,&rcvbuf,sizeof(char)*MSG_SIZE,0);
	//recv(clientSocket,&uint32_t_buf, PACKET_SIZE, 0);
	//recv(clientSocket,&recv_buf, PACKET_SIZE, 0);


  /*---- deserialze ----*/
		deserialize_msg(msgbuf,rcvbuf);
//		deserialize_uint32_t(&testint,uint32_t_buf);
//	deserializeIRCPacket(&test,&recv_buf);	
  
	/*---- Print the received message ----*/
//  printf("Data received: %u\n",testint);
  printf("Data received: %s\n",msgbuf);
//	printf("%u\n%s\n",test.id,test.msg);

}
