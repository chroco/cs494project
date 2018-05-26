#include "client.h"

IRCClient::IRCClient():clientSocket(0){}

IRCClient::~IRCClient(){}

int IRCClient::joinServer(){
	IRCPacket test = {0u,0u,0u,{"hello test packet!\0"}},
						irc_msg = {0,0,0,0},
						echo = {0,0,0,0};
	char eom='\0',
			 sendbuf[IRC_PACKET_SIZE]={0},
			 recvbuf[IRC_PACKET_SIZE]={0},
			 tokenbuf[BUFLEN+1]={0};
	int bytes_sent = 0, bytes_recv = 0;

	int sock;
	struct sockaddr_in server;
	char message[BUFLEN+1]={0};// , server_reply[BUFLEN+1];

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1){
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr(HOME);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	//keep communicating with server
	while(1){
		printf("Enter message : ");
//		scanf("%s" , message);
		scanf("%2000[^\n]%c", message,&eom);
		eom='\0';

		if(message[0]!='/'){
			printf("issuing command!\n");
		
			strcpy(tokenbuf,message);
			char* token = strtok(tokenbuf, " ");
			while (token) {
				printf("token: %s\n", token);
				token = strtok(NULL, " ");
			}
			memset(&message,0,sizeof(message));
			memset(&tokenbuf,0,sizeof(tokenbuf));
			continue;
		}

		//Send some data
		serializeIRCPacket(sendbuf,&test);	
		bytes_sent=send(sock,sendbuf,IRC_PACKET_SIZE,0);
		if(bytes_sent < 0){
			fprintf(stderr, "error sending...");
		}

		//Receive a reply from the server
		bytes_recv = recv(sock,recvbuf,IRC_PACKET_SIZE,0);
		if(bytes_recv < 0){
			fprintf(stderr, "error receiving...");
		}

		deserializeIRCPacket(&echo,recvbuf);	
		printf("bytes_sent: %d\n",bytes_sent);
		printf("bytes_recv: %d\n",bytes_recv);
		printf("%u,%u,%u\n%s\n",echo.p.length,echo.p.op_code,echo.p.error_code,echo.p.msg);
		memset(&echo,0,IRC_PACKET_SIZE);
		memset(&sendbuf,0,IRC_PACKET_SIZE);
		memset(&recvbuf,0,IRC_PACKET_SIZE);
	}

	close(sock);

	return 0;
}

int IRCClient::base_joinServer(){
	int sock;
	struct sockaddr_in server;
	char message[BUFLEN+1] , server_reply[BUFLEN+1];

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1){
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr(HOME);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	//keep communicating with server
	while(1){
		printf("Enter message : ");
		scanf("%s" , message);

		//Send some data
		if( send(sock , message , strlen(message) , 0) < 0){
			puts("Send failed");
			return 1;
		}

		//Receive a reply from the server
		if( recv(sock , server_reply , 2000 , 0) < 0){
			puts("recv failed");
			break;
		}

		puts("Server reply :");
		puts(server_reply);
	}

	close(sock);
	return 0;
}

//*
void IRCClient::requestConnection(){
	IRCPacket test = {0u,0u,0u,{"hello test packet!\0"}},
						echo = {0,0,0,0};
	char sendbuf[IRC_PACKET_SIZE]={0};
	char recvbuf[IRC_PACKET_SIZE]={0};
	int bytes_sent = 0, bytes_recv = 0;

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr(HOME);
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  addr_size = sizeof serverAddr;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	
	serializeIRCPacket(sendbuf,&test);	
	bytes_sent=send(clientSocket,sendbuf,IRC_PACKET_SIZE,0);
	printf("bytes_sent: %d\n",bytes_sent);
	bytes_recv = recv(clientSocket,recvbuf,IRC_PACKET_SIZE,0);
	if(bytes_recv < 0){
		fprintf(stderr, "error receiving...");
		return;
	}
	deserializeIRCPacket(&echo,recvbuf);	
	printf("bytes_sent: %d\n",bytes_sent);
	printf("bytes_recv: %d\n",bytes_recv);
	printf("%u,%u,%u\n%s\n",echo.p.length,echo.p.op_code,echo.p.error_code,echo.p.msg);
	close(clientSocket);
}
//*/	

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
	printf("%u,%u,%u\n%s\n",test.p.length,test.p.op_code,test.p.error_code,test.p.msg);
}
