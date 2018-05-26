#ifndef _ECHO_H_
#define _ECHO_H_

#include <stdio.h>		/* Basic I/O routines 		*/
#include <sys/types.h>		/* standard system types 	*/
#include <netinet/in.h>		/* Internet address structures 	*/
#include <sys/socket.h>		/* socket interface functions 	*/
#include <netdb.h>		/* host to IP resolution 	*/
#include <sys/time.h>		/* for timeout values 		*/
#include <unistd.h>		/* for table size calculations 	*/
#include <string.h> 
#include <arpa/inet.h>

#define	PORT		5060	/* port of our echo server */
#define	BUFLEN		1024	/* buffer length 	   */

#endif
