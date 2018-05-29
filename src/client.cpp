#include "client.h"

IRCClient::IRCClient():clientSocket(0){}

IRCClient::~IRCClient(){}

/*
int receive_basic(char *chunk,int s)
{
    int size_recv , total_size= 0;
    //char chunk[CHUNK_SIZE];
     
    //loop
    while(1)
    {
        memset(chunk ,0 , IRC_PACKET_SIZE);  //clear the variable
        if((size_recv =  recv(s , chunk , IRC_PACKET_SIZE , 0) ) < 0)
        {
            break;
        }
        else
        {
            total_size += size_recv;
            printf("%s" , chunk);
        }
    }
     
    return total_size;
}
//*/

int IRCClient::joinServer(){
//	IRCPacket test = {0u,0u,0u,{"hello test packet!\0"}},
		IRCPacket	irc_msg = {0,0,0,0},
							echo = {0,0,0,0};
	char eom='\0',
			 *cmd[CMD_SIZE]={NULL},
			 sendbuf[IRC_PACKET_SIZE]={0},
			 recvbuf[IRC_PACKET_SIZE]={0};
	int bytes_sent = 0, bytes_recv = 0;

	int sock,i;
	struct sockaddr_in server;
	char message[BUFLEN+1]={0};

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
		printf("Type things o_0: ");
		scanf("%2000[^\n]%c", message,&eom);
		eom='\0';

		if(message[0]=='/'){
			printf("Issuing command: ");
		
			char* token = strtok(message, " ");
			for(i=0;token!=NULL;++i){
				printf("%s ",token);
				cmd[i]=token;
				token = strtok(NULL," ");
			}
			printf("\n");
			for(int i=0;i<=END_OF_COMMANDS;++i){
				if(i==END_OF_COMMANDS){
					fprintf(stderr,"command format error!\n");
				}
				if(strcmp(cmd[0],command[i])==0){
					irc_msg.p.op_code=i;
					break;
				}
			}
			if(cmd[1]){
				strcpy(irc_msg.p.msg,cmd[1]);
			}
		}else{
			continue;	
		}

		memset(&message,0,sizeof(message));
		
		//Send some data
		serializeIRCPacket(sendbuf,&irc_msg);	
		bytes_sent=send(sock,sendbuf,IRC_PACKET_SIZE,0);
		if(bytes_sent != IRC_PACKET_SIZE){
			fprintf(stderr, "error sending...\n");
		}

		//Receive a reply from the server
		bytes_recv = recv(sock,recvbuf,IRC_PACKET_SIZE,0);
		if(bytes_recv != IRC_PACKET_SIZE){
			fprintf(stderr, "error receiving...\n");
		}

		deserializeIRCPacket(&echo,recvbuf);	
		printf("bytes_sent: %d\n",bytes_sent);
		printf("bytes_recv: %d\n",bytes_recv);
		printf("%u,%u,%u\n%s\n",
				echo.p.length,echo.p.op_code,echo.p.error_code,echo.p.msg
		);
		memset(&echo,0,IRC_PACKET_SIZE);
		memset(&sendbuf,0,IRC_PACKET_SIZE);
		memset(&recvbuf,0,IRC_PACKET_SIZE);
	}

	close(sock);

	return 0;
}

