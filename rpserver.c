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

int main(int argc, char **argv){

	int port = 3706;
	printf("hello");
	serve(port);	
	return 0;
}

void serve(int port){

	int svc;
	int rqst;
	socklen_t alen;
	struct sockaddr_in my_addr;
	struct sockaddr_in client_addr;
	int sockoptval = 1;
	char hostname[128];
	
	//hostname = my computers name
	gethostname(hostname, 128);

	//create tcp/ip socket
	//socket(domain, type, protocol)
	if ((svc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("cannot create socket");
		exit(1);
	}

	//allows the reuse of a port immediately after the service exits
	setsockopt(svc, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));

	//setup our address
	memset((char*)&my_addr, 0, sizeof(my_addr));		//0's the struct
	my_addr.sin_family = AF_INET;		//family = AF_INET
	my_addr.sin_port = htons(port);		//port = port that we delcared, host to network (short)
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//INADDR_ANY = 0.0.0.0

	//bind socket
	//bind(int socket, 
	if (bind(svc, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0){
		perror("bind failed");
		exit(1);
	}

	printf("server started on %s, listening on port %d\n", hostname, port);

}
