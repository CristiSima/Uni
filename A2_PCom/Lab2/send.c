#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"
#include "packet.h"

/* We don't touch this */
#define HOST "127.0.0.1"
#define PORT 10000

#define DLE (char)0
#define STX (char)2
#define ETX (char)3

/* TODO 2: implement send_frame function */
void send_frame(char* frame, int frame_size)
{
	send_byte(DLE);
	send_byte(STX);
	
	for(int i=0;i<frame_size;i++)
	{
		if(frame[i]==DLE)
			send_byte(DLE);
		send_byte(frame[i]);
	}

	send_byte(DLE);
	send_byte(ETX);
}

int main(int argc,char** argv){
	init(HOST,PORT);

	/* Send Hello */
	send_byte(DLE);
	send_byte(STX);
	send_byte('H');
	send_byte('e');
	send_byte('l');
	send_byte('l');
	send_byte('o');
	send_byte('!');

	/* TODO 2: call send_frame function with a given string input */
	send_frame("1234", 4);
	//send_frame("0123456789AAB", 13);

	//send_frame("0AAAAAAAAAAAB", 13);
	//send_frame("0XXXXXXXXXXBB", 13);
	//send_frame("0YYYYYYYYYBBB", 13);
	send_frame("DONE", 5);


	/* TODO 3: use send_frame to send a structure of type Packet */

	struct Packet p={.size=5, .payload="ABCDE", .sum=2};
	send_frame(&p, sizeof p);
	
	// usleep(1000000);




	
	/* TODO 4: send 100 bytes, send 300 bytes, append a timestamp to these frames */	
	
	

	return 0;
}
