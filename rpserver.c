//rpserver: remote popen server
//Luke Xu : 134003706
//Utkarsha Mavani
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <sys/wait.h>

#include "port.h"

#ifndef ERESTART
#define ERESTART EINTR
#endif

extern int errno;

void serve(int port);
void receive_command(int sock);

int main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int port = SERVICE_PORT;
	static char usage[] = "usage: %s [-p port]\n";
	
	if (argc == 3){
		if (strcmp(argv[1],"-p")==0){
			if (atoi(argv[2])!=0){
				port = atoi(argv[2]);
			}
			else{
				fprintf(stderr, usage, argv[0]);
				exit(1);
			}
		}
		else{
			fprintf(stderr, usage, argv[0]);
			exit(1);
		}
	}
	else if (argc == 1){
		char* envPort;
		envPort = getenv("PPORT");
		if (envPort == NULL){
			port = SERVICE_PORT;
		}
		else{
			port = atoi(envPort);
		}
	}
	else{
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}

	serve(port);
}

void serve(int port){
    
	int svc;        //socket listening
	int rqst;       //socket accepting request
	socklen_t alen;       //length of address
	struct sockaddr_in my_addr;    //server address
	struct sockaddr_in client_addr;  //client address
	int sockoptval = 1;
	char hostname[128];

    //our hostname eg. luke's air
	gethostname(hostname, 128);
    
    //create tcp/ip socket
    //socket(domain, type, protocol)
	if ((svc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("cannot create socket");
		exit(1);
	}
    
    //allows us to reuse port immediately after the service exits
	setsockopt(svc, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));

    //setup our address
	memset((char*)&my_addr, 0, sizeof(my_addr));  /* 0 out the structure */
	my_addr.sin_family = AF_INET;   /* address family */
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind address to socket
    //bind(int socket, const struct sockaddr *address, socklen_t address_len)
	if (bind(svc, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
		perror("bind failed");
		exit(1);
	}

	//listen, max queue = 5
	if (listen(svc, 5) < 0) {
		perror("listen failed");
		exit(1);
	}
    
	printf("server started on %s, listening on port %d\n", hostname, port);

	/* loop forever - wait for connection requests and perform the service */
	alen = sizeof(client_addr);     //length of address

	for (;;) {
		while ((rqst = accept(svc,
		                (struct sockaddr *)&client_addr, &alen)) < 0) {
            //break if syscall was interrupted
			if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
				perror("accept failed");
				exit(1);
			}
		}

		printf("received a connection from: %s port %d\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	
		//END TEST
		receive_command(rqst);	
		//END NEW	
		printf("closing socket %d\n", rqst);
        close(rqst); //close connection
	}
}


void receive_command(int sock){	

		
	switch(fork()){
	case -1:
		fprintf(stderr, "fork failed!\n");
		exit(1);
	default:
		return;
	case 0:
		break;
	}
	
	char command[BSIZE];
	int nbytes;
	
	nbytes = read(sock, command, BSIZE);
	
	if (nbytes < 0){
		fprintf(stderr, "no command from received from client\n");
	}
	else if(nbytes == 0){
		printf("no arguement recieved from client\n");
		exit(1);
	}
	else{
		command[nbytes] = 0;
		printf("command: \"%s\"\n", command);		
		close(0);
		close(1);
		dup2(sock, 1);
		execlp("/bin/sh", "/bin/sh", "-c", command, (char*)0);
		fprintf(stderr, "uasdfnknown command: %s", command);
		exit(1);
	}
}

