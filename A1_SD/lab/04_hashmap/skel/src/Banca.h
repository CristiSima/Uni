#include "Hashtable.h"

#define TRANSACTION_PLACE_LEN 51
#define CLIENT_NAME_LEN 51

typedef struct client_t client_t;
struct client_t {
	char name[CLIENT_NAME_LEN];
	unsigned int pin;
	linked_list_t *transactions;
	int balance;
};

typedef unsigned short ushort;
typedef unsigned int uint;
typedef struct transaction_t transaction_t;
struct transaction_t {
	char place[TRANSACTION_PLACE_LEN];
	unsigned short day,month,year;
	int ammount;
};

// nota: ar ere stocata functia de free pt data

typedef void (*free_function_t)(void*);
