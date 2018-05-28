#include "client.h"

IRCClient::IRCClient():clientSocket(0){}

IRCClient::~IRCClient(){}

int IRCClient::joinServer(){
	IRCPacket test = {0u,0u,0u,{"hello test packet!\0"}},
						irc_msg = {0,0,0,0},
						echo = {0,0,0,0};
	char eom='\0',
			 *cmd[CMD_SIZE]={NULL},
			 sendbuf[IRC_PACKET_SIZE]={0},
			 recvbuf[IRC_PACKET_SIZE]={0};
	int bytes_sent = 0, bytes_recv = 0;

	int sock,i;
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
		scanf("%2000[^\n]%c", message,&eom);
		eom='\0';

		if(message[0]=='/'){
			printf("issuing command!\n");
		
			char* token = strtok(message, " ");
			for(i=0;token!=NULL;++i){
				printf("token: %s\n",token);
				cmd[i]=token;
				token = strtok(NULL," ");
			}
			printf("cmd[0]: %s\n",cmd[0]);
			printf("cmd[1][0]: %c\n",cmd[1][0]);
			if(strcmp(cmd[0],"/join")==0&&cmd[1]!=NULL&&cmd[1][0]=='#'){
				irc_msg.p.op_code=1;
				strcpy(irc_msg.p.msg,cmd[1]);
			}//else{
			//	fprintf(stderr,"/join command format error!\n");
			//	continue;
		//	}
			else if(strcmp(cmd[0],"/part")==0&&cmd[1]!=NULL&&cmd[1][0]=='#'){
				irc_msg.p.op_code=2;
				strcpy(irc_msg.p.msg,cmd[1]);
			}else{
				fprintf(stderr,"command format error!\n");
				continue;
			}
		}else{
			
		}

		memset(&message,0,sizeof(message));
		//Send some data
		serializeIRCPacket(sendbuf,&irc_msg);	
		//serializeIRCPacket(sendbuf,&test);	
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

