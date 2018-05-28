#include "echo.h"

/* multi-client-echo-server.c - a multi-client echo server */

int main(){
	int	i,rc,s,cs,dsize; 
	char buf[BUFLEN+1];  
	struct sockaddr_in sa; 
	struct sockaddr_in csa;
	socklen_t size_csa;
	fd_set rfd; 	
	fd_set c_rfd; 

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	sa.sin_addr.s_addr = INADDR_ANY;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		perror("socket: allocation failed");
	}

	rc = bind(s, (struct sockaddr *)&sa, sizeof(sa));

	if (rc) {
		perror("bind");
	}

	printf("listening...\n");
	rc = listen(s, 5);

	if (rc) {
		perror("listen");
	}

	size_csa = sizeof(csa);
	dsize = getdtablesize();
	
	for (i=3; i<dsize; i++){
		if (i != s){
			close(i);
		}
	}
	printf("closed FDs...\n");

	FD_ZERO(&rfd);
	FD_SET(s, &rfd);
	
	while (1){
		c_rfd = rfd;
		printf("selecting...");
		rc = select(dsize, &c_rfd, NULL, NULL, (struct timeval *)NULL);
		printf("\n");

		if (FD_ISSET(s, &c_rfd)) {
			cs = accept(s, (struct sockaddr *)&csa, &size_csa);
			if (cs < 0){
				continue;
			}

			/* add the new socket to the set of open sockets */
			FD_SET(cs, &rfd);

			/* and loop again */
			continue;
		}

		for (i=3; i<dsize; i++){
			if (i != s && FD_ISSET(i, &c_rfd)){
				/* read from the socket */
				rc = read(i, buf, BUFLEN);
				/* if client closed the connection... */
				if (rc == 0) {
					/* close the socket */
					close(i);
					FD_CLR(i, &rfd);
				}else {/* if there was data to read */
					/* echo it back to the client */
				//	write(i, "hello server!", rc);
					write(i, "hello server!", BUFLEN);
				}
			}
		}
	}
	return 0;
}
