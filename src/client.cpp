#include "client.h"
#include <pthread.h>

IRCClient::IRCClient():clientSocket(0){}

IRCClient::~IRCClient(){}

void *IRCClient::recvMessage(void *ptr){
	IRCPacket echo = {0,0,0,0};
	char recvbuf[IRC_PACKET_SIZE]={0};
	int bytes_recv = 0,
			*sock = (int *)ptr;

	while(1){
		//Receive a reply from the server
		bytes_recv = recv(*sock,recvbuf,IRC_PACKET_SIZE,0);
		if(bytes_recv == 0){
			fprintf(stderr, "Lost connection to server...\n");
			exit(0);
		}else if(bytes_recv != IRC_PACKET_SIZE){
			fprintf(stderr, "error receiving...\n");
			exit(0);
		}

		deserializeIRCPacket(&echo,recvbuf);	
		printf("\nbytes_recv: %d\n",bytes_recv);
		printf("%u,%u,%u\n%s\n",
				echo.p.length,echo.p.op_code,echo.p.error_code,echo.p.msg
		);
		//fprintf(stderr,"Type things o_0: ");
		printf("Type things o_0: ");
		fflush(stdout);
		memset(&echo,0,IRC_PACKET_SIZE);
		memset(&recvbuf,0,IRC_PACKET_SIZE);
	}
}

IRCClient *pIRCClient;

void *recvMessageWrapper(void *ptr){
	pIRCClient->recvMessage(ptr);
	return NULL;
}

int IRCClient::joinServer(){
	IRCPacket	irc_msg = {0,0,0,0};
	char eom='\0',
			 cmd[CMD_SIZE]={'\0'},
			 message[BUFLEN]={0},
			 sendbuf[IRC_PACKET_SIZE]={0};
	int sock,i,bytes_sent = 0;//, bytes_recv = 0;

	struct sockaddr_in server;
	pthread_t recv_thread;

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

	if(pthread_create(&recv_thread,NULL,recvMessageWrapper,&sock)){
		fprintf(stderr,"Cannot create thread...\n");
		return 1;
	}

	//keep communicating with server
	printf("Type things o_0: ");
	while(1){
		memset(&message,0,sizeof(message));
		memset(&cmd,0,sizeof(cmd));
		scanf("%2000[^\n]%c", message,&eom);
		eom='\0';

		if(message[0]=='/'){
			printf("Issuing command: ");
			truncateFirstWord(cmd,message,CMD_SIZE);
			printf("\n");
			for(i=0;i<=END_OF_COMMANDS;++i){
				if(i==END_OF_COMMANDS){
					fprintf(stderr,"command format error!\n");
				}
				if(strcmp(cmd,command[i])==0){
					irc_msg.p.op_code=i;
					break;
				}
			}
			if(cmd[1]){
				strcpy(irc_msg.p.msg,message);
			}
		}else{
			printf("Type things o_0: ");
			continue;	
		}

		//Send some data
		serializeIRCPacket(sendbuf,&irc_msg);	
		bytes_sent=send(sock,sendbuf,IRC_PACKET_SIZE,0);
		if(bytes_sent != IRC_PACKET_SIZE){
			fprintf(stderr, "error sending...\n");
		}
		printf("bytes_sent: %d\n",bytes_sent);
		switch(irc_msg.p.op_code){
			case MSG:
				printf("Type things o_0: ");
				break;
			case EXIT:
				close(sock);
				printf("Exiting...\n");
				return 0;
			default:
				break;
		}
		memset(&sendbuf,0,IRC_PACKET_SIZE);
	}
	close(sock);
	return 0;
}

