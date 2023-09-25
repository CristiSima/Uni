#ifndef _HELPERS_H
#define _HELPERS_H 1

#include <stdio.h>
#include <stdlib.h>

/*
 * Macro de verificare a erorilor
 * Exemplu:
 *     int fd = open(file_name, O_RDONLY);
 *     DIE(fd == -1, "open failed");
 */

#define DIE(assertion, call_description)	\
	do {									\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",	\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(EXIT_FAILURE);				\
		}									\
	} while(0)

// 	topic +	data_tpye_identifier + 	content
//	50	  +			1			 +	1500	= 1551
// UDP_CLIENT: port+addr	= 6
#define BUFLEN 1600

#define TCP_NODELAY 1

#define INFO_DATA_TYPE_INT 0
#define INFO_DATA_TYPE_SHORT_REAL 1
#define INFO_DATA_TYPE_FLOAT 2
#define INFO_DATA_TYPE_STRING 3

char *INFO_DATA_TYPE_NAME[]={"INT",
							"SHORT_REAL",
							"FLOAT",
							"STRING"
							};



#define TOPIC_LEN 50
#define CONTENT_LEN 1500

#define INFO_INFO 1
#define INFO_CMD 2
#define INFO_REPLY 3
#define INFO_GET_SF 4

struct info_msg
{
	char topic[TOPIC_LEN];
	uint8_t data_type;
	uint8_t content[CONTENT_LEN];
} __attribute__((packed));

struct info_packet_header
{
	uint8_t packet_type;
	// length of the remaining bytes
	uint16_t len;
} __attribute__((packed));
#define INFO_HEADER_SIZE (sizeof(struct info_packet_header))


struct info_msg_info
{
	// for smart pointer
	uint32_t uses;
	// length of data to send
	uint16_t len;

	struct info_packet_header header;
	struct sockaddr_in src_addr;
	struct info_msg msg_only;
} __attribute__((packed));


#define INFO_REPLY_MAX_LEN 500
// #define INFO_REPLY_BUILD(msg) "\xINFO_REPLY" msg
// #define INFO_REPLY_BUILD(msg) "\x03" #(sizeof msg )msg

//  !!!!!!
// AT A NEW CALL THE OLD VALUE WILL BE CORUPTED
//  !!!!!!
static char *INFO_REPLY_BUILD(char *msg)
{
	static char info_reply[INFO_REPLY_MAX_LEN+1+sizeof(struct info_packet_header)]={INFO_REPLY,};
	// copy len
	uint16_t len=htons(strlen(msg));
	memcpy(info_reply+1, &len, sizeof(uint16_t));
	// copy string
	strncpy(info_reply+INFO_HEADER_SIZE, msg, INFO_REPLY_MAX_LEN);

	return info_reply;
}
#endif
