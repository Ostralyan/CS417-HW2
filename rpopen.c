/*
	rpopen: remote popen function
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

int conn(char *host, int port);
void disconn(void);
FILE* rpopen(char *host, int port, char *cmd);


int main(){
	printf("hello");	
	return 0;
}

FILE *rpopen(char *host, int port, char *cmd){

	return 0;
}


int fd;

int conn(char *host, int port){
	
	struct hostent *hp;		//host info
	unsigned int alen;		//address length of port num
	struct sockaddr_in myaddr;		//our address
	struct sockaddr_in servaddr;	//server address

	printf("conn(host=\"%s\", port=\"%d\")\n", host, port);

	//create tcp/ip socket
	//socket(domain, type, protocol)
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("cannot create socket");
		return 0;
	}

	//setup our address 
	memset((char*)&myaddr, 0, sizeof(myaddr));		//0s the struct
	myaddr.sin_family = AF_INET;		//family = AF_INET
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);	//INADDR_ANY = 0.0.0.0, host to network (long)
	myaddr.sin_port = htons(0);		//port = 0, host to network (short)
	
	//bind socket
	//bind(int socket, const struct sockaddr *address, socklen_t address_len)
	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0){
		perror("bind failed");
		return 0;
	}

	//debugging only
	alen = sizeof(myaddr);
	if (getsockname(fd, (struct sockaddr *)&myaddr, &alen) < 0){
		perror("getsockname failed");
		return 0;
	}
	printf("local port number = %d\n", ntohs(myaddr.sin_port));
	//end debug
	
	//setup server address	
	memset((char*)&servaddr, 0, sizeof(servaddr));		//0s the struct
	servaddr.sin_family = AF_INET;		//family = AF_INET
	servaddr.sin_port = htons(port);		//port = port of server, host to network (short) 
	

	
	return 0;
}

void disconn(void){

	printf("disconn()\n");
	close(fd);
}
