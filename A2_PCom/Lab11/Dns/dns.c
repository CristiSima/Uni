// Protocoale de comunicatii
// Laborator 9 - DNS
// dns.c

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char* name)
{
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char* name)
{
	int ret;
	char buff[101]={0};
	struct addrinfo hints, *result, *p;

	// TODO: set hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags=0;
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=0;
	hints.ai_addr=NULL;
	hints.ai_canonname=NULL;
	hints.ai_next=NULL;
	// TODO: get addresses
	ret=getaddrinfo(name, NULL, &hints, &result);
	printf("%d\n", ret);

	// TODO: iterate through addresses and print them
	for(p=result;p;p=p->ai_next)
	{
		struct sockaddr_in *serv_addr=p->ai_addr;
		printf("%08x\t", serv_addr->sin_addr);
		puts(inet_ntop(AF_INET, p->ai_addr, buff, p->ai_addrlen));
		puts(inet_ntop(AF_INET, &serv_addr->sin_addr, buff, p->ai_addrlen));

		//char *ip_s=inet_ntop(AF_INET, 
	}

	// TODO: free allocated data
	freeaddrinfo(result);
}

// Receives an address and prints the associated name and service
void get_name(char* ip)
{
	int ret;
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

	// TODO: fill in address data
	memset(&addr, 0, sizeof(addr));
	addr.sin_family=AF_INET;
	//hints.=SOCK_STREAM;
	ret=inet_aton(ip, &addr.sin_addr);
	sock

	// TODO: get name and service
	ret=getnameinfo(
	// TODO: print name and service
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0) {
		get_ip(argv[2]);
	} else if (strncmp(argv[1], "-a", 2) == 0) {
		get_name(argv[2]);
	} else {
		return usage(argv[0]);
	}

	return 0;
}
