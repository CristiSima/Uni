#include "common.h"

uint16_t inet_csum(uint8_t *buf, size_t len) {
	//TODO1: Calculate the internet checksum according to RFC1071
	uint32_t sum=0;
	int i=0;
	for(i;i+1<len;i+=2)
		sum+=*(uint16_t*)(buf+i);
	if(i<len)
		sum+=*(uint8_t*)(buf+i);

	while(sum>(1<<16))
		sum=(sum&0xffff)+(sum>>16);

	return sum;
}
