#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"
#include "packet.h"

#define HOST "127.0.0.1"
#define PORT 10001

#define DLE (char)0
#define STX (char)2
#define ETX (char)3

/* TODO 2: write recv_frame function */
int recv_frame(char* buffer, int max_size)
{
	char c;

	do
	{
		if(recv_byte() == DLE) if( recv_byte()==STX)
			break;
	} while(1);

	for(int i=0;i<max_size;i++)
	{
		c=recv_byte();
		if(c==DLE)
			switch(recv_byte())
			{
				case ETX:
					return i;
				case DLE:
					buffer[i]=DLE;
					break;
				default:
					return -1;
			}
		else
			buffer[i]=c;
	}
	return max_size;
}

int main(int argc,char** argv){
  init(HOST,PORT);
  

  char c;

  /* Wait for the start of a frame */
  char c1,c2;
  do {
	c1 = recv_byte();
	c2 = recv_byte();
  } while(c1 != DLE || c2 != STX);
  
  printf("%d ## %d\n",c1, c2);
  c = recv_byte();
  printf("%c\n", c);

  c = recv_byte();
  printf("%c\n", c);

  c = recv_byte();
  printf("%c\n", c);

  c = recv_byte();
  printf("%c\n", c);

  c = recv_byte();
  printf("%c\n", c);

  c = recv_byte();
  printf("%c\n", c);


  char buffer[MAX_LEN];
  int size;
  struct Packet p;


  do
  {
	memset(buffer, 0, 100);
  	size=recv_frame(buffer, MAX_LEN);
	printf("%d|%s\n", size, buffer);  
  }
  while(!strcmp(buffer, "DONE"));


  recv_frame(&p, sizeof p);
  printf("{size:%d, p:%s, sum:%d}\n", p.size, p.payload, p.sum);  
  
  
  /* TODO 2: Run the receiver until you receive the frame DONE */

  /* TODO 3: receive a frame with a structure of type Packet */

  /* TODO 4: Measure latency in a while loop for any frame that contains a timestamp we receive, print frame_size and latency */

  printf("[RECEIVER] Finished transmission\n");
  return 0;
}
