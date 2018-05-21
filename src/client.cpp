#include "client.h"

IRCClient::IRCClient():clientSocket(0){}

IRCClient::~IRCClient(){}

void IRCClient::requestConnection(){
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr(HOME);
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	
	char recvbuf[5]={0};
	int bytes_recv = recv(clientSocket,recvbuf,5,0);
	if(bytes_recv < 0){
		fprintf(stderr, "error receiving...");
		return;
	}
	fprintf(stderr, "bytes_recv: %d\n",bytes_recv);
	fprintf(stderr, "recvbuf: %s\n",recvbuf);
	
}

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
	IRCPacket test;
	char recvbuf[IRC_PACKET_SIZE]={0};
	int bytes_recv = recv(clientSocket,recvbuf,IRC_PACKET_SIZE,0);
	if(bytes_recv < 0){
		fprintf(stderr, "error receiving...");
		return;
	}
	fprintf(stderr, "bytes_recv: %d\n",bytes_recv);

	/*---- Deserialize message ----*/
	deserializeIRCPacket(&test,recvbuf);	
  
	/*---- Print the received message ----*/
	printf("%u\n%s\n",test.p.id,test.p.msg);

}
