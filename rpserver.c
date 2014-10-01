/*
	rpserver: remote popen server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <sys/wait.h>

extern int errno;
void serve(int port);

main(int argc, char **argv){

	int port = 3706;
	
	serve(port);	
}

void serve(int port){

	int svc;
	int rqst;
	socklen_t alen;
	struct sockaddr_in my_addr;
	struct sockaddr_in client_addr;
	int sockoptval = 1;
	char hostname[128];

	gethostname(hostname, 128);

	//create socket
	if ((svc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("cannot create socket");
		exit(1);
	}

	



}
