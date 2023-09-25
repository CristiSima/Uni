#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"


void usage(char *file)
{
	fprintf(stderr, "Usage: %s ID_CLIENT server_address server_port\n", file);
	exit(0);
}

void show_info(struct sockaddr_in *src_addr, struct info_msg *msg_only, uint8_t msg_size)
{
	// when topic has TOPIC_LEN chars there is no \0
	char topic_buff[TOPIC_LEN+1]={0};
	strncpy(topic_buff, msg_only->topic, TOPIC_LEN);

	// base
	printf("%s:%d - %s - %s - ", inet_ntoa(src_addr->sin_addr), ntohs(src_addr->sin_port),
								topic_buff, INFO_DATA_TYPE_NAME[msg_only->data_type]);

	switch (msg_only->data_type)
	{
	    case INFO_DATA_TYPE_INT:
	    {
	        uint32_t val=ntohl(*(uint32_t*)&msg_only->content[1]);

	        if(msg_only->content[0]==0)
	            printf("%d\n", val);
	        else
	            printf("-%d\n", val);

	        break;
	    }
	    case INFO_DATA_TYPE_SHORT_REAL:
	    {
	        uint16_t val=ntohs(*(uint16_t*)&msg_only->content[0]);
	        printf("%u.%02u\n", val/100, val%100);
	        break;
	    }
	    case INFO_DATA_TYPE_FLOAT:
	    {
	        uint32_t val=ntohl(*(uint32_t*)&msg_only->content[1]);
	        uint32_t pow=1;

	        for(uint8_t i=msg_only->content[1+4];i;i--)
	            pow*=10;

	        // 8:base_len   4:more then 255     1: \0
	        char pattern[8+4+1]={0};
	        // build pattern for preceding 0s
	        sprintf(pattern,"-%%u.%%0%du\n", msg_only->content[1+4]);

	        uint8_t is_pos=msg_only->content[0]==0;
	        printf(pattern+is_pos, val/pow, val%pow);

	        break;
	    }
	    case INFO_DATA_TYPE_STRING:
	    {
			// when content_string has CONTENT_LEN chars there is no \0
			char content_buff[CONTENT_LEN+1]={0};
			strncpy(content_buff, (char*)msg_only->content, CONTENT_LEN);

			content_buff[msg_size-TOPIC_LEN-1]=0;

	        puts(content_buff);
	        break;
	    }
	    default:
	        printf("\n%d ", msg_only->data_type);
	        DIE(1, "UNKNOWN INFO_DATA_TYPE");
	}
}

int main(int argc, char *argv[])
{
	int sockfd, n, rc;
    int msg_size;
	char buffer[BUFLEN];
	struct info_packet_header header;
	struct sockaddr_in serv_addr;
	struct info_msg_info info;

	fd_set read_fds;
	fd_set ready_fds;

	// call check
	if (argc < 4) {
		usage(argv[0]);
	}

	// dissable buffering
	rc=setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	DIE(rc!=0, "setvbuf");

	// get socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	rc=1;
    rc=setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &rc, sizeof(int));
    DIE(rc < 0, "setsockopt TCP_NODELAY");

	// init sets
	FD_ZERO(&read_fds);
	FD_SET(sockfd, &read_fds);
	FD_SET(0, &read_fds);

	// construct server address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	rc = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(rc == 0, "inet_aton");

	// connect
	rc = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(rc < 0, "connect");

	// send client id
	rc = send(sockfd, argv[1], strlen(argv[1]), 0);
	DIE(rc<=0, "send client_id");

	msg_size=rc=recv(sockfd, buffer, sizeof(buffer) - 1, 0);
	DIE(rc<=0, "recv connection status");
	buffer[msg_size]=0;


	if(strncmp("Connected.", buffer, 10))
	{
		// failed to connect

		printf("%s\n", buffer);
		close(sockfd);
		return 0;
	}

	// request Store & Forward
	header.packet_type=INFO_GET_SF;
	header.len=0;
	n = send(sockfd, &header, sizeof(header), 0);


	int on=1;
	while (on) {
  		// se citeste de la stdin
		ready_fds=read_fds;
		rc=select(sockfd+1, &ready_fds, NULL, NULL, NULL);

		if(FD_ISSET(0, &ready_fds))
		{
			// stdin
			memset(buffer, 0, sizeof(buffer));
			n = read(0, buffer, sizeof(buffer) - 1);
			DIE(n < 0, "read");

			// preaper packet
			header.packet_type=INFO_CMD;

			buffer[n]=0;

			// send multiple commands separatly
			char *command=strtok(buffer, "\n");
			while(command!=NULL)
			{

				header.len=htons(strlen(command));
				// send command

				n = send(sockfd, &header, sizeof(header), 0);
				DIE(n < 0, "send");

				n = send(sockfd, command, strlen(command), 0);
				DIE(n < 0, "send");

				// check if exit
				if (!strncmp(command, "exit", 4))
				{
					break;
				}

				command=strtok(NULL, "\n");
			}

		}
		else if(FD_ISSET(sockfd, &ready_fds))
		{
			// remote server
			memset(buffer, 0, sizeof(buffer));
			msg_size = recv(sockfd, &header, sizeof(header), 0);
			if(msg_size<=0) break;

			header.len=ntohs(header.len);

			switch(header.packet_type)
			{
				case INFO_INFO:
				{
					msg_size = recv(sockfd, &info.src_addr, (uint32_t)header.len, 0);
					DIE(n < 0, "send");

					show_info(&info.src_addr, &info.msg_only, msg_size-sizeof(info.src_addr));

					break;
				}
				case INFO_REPLY:
				{
					msg_size = recv(sockfd, buffer, (uint32_t)header.len, 0);
					DIE(n < 0, "send");

					buffer[msg_size]=0;

					printf("%s\n", buffer);

					break;

				}
				default:
				{
					printf("GOT %d bytes TYPE:%d\n", msg_size, buffer[0]);

					msg_size = recv(sockfd, buffer, (uint32_t)header.len, 0);
					DIE(n < 0, "send");
				}
			}
		}
	}

	close(sockfd);
	return 0;
}
