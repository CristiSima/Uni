#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TOKEN_LEN 15
#define USER_ID_LEN 15
#define RESOURCE_MAX_LEN 100

#define REQUEST_AUTHORIZATION_GRANTED 0
#define REQUEST_AUTHORIZATION_USER_NOT_FOUND 1

#define REQUEST_ACCESS_TOKEN_GRANTED 0
#define REQUEST_ACCESS_TOKEN_DENIED 1


#define VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED			0
#define VALIDATE_DELEGATED_ACTION_TOKEN_EXPIRED				1
#define VALIDATE_DELEGATED_ACTION_RESOURCE_NOT_FOUND		2
#define VALIDATE_DELEGATED_ACTION_OPERATION_NOT_PERMITTED	3
#define VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED		4
const char *validate_delegated_action_result_code_repr[] = {
	"PERMISSION_DENIED",
	"TOKEN_EXPIRED",
	"RESOURCE_NOT_FOUND",
	"OPERATION_NOT_PERMITTED",
	"PERMISSION_GRANTED",
	NULL
};

/**
 * generate alpha-numeric string based on random char*
 *
 * INPUT: fixed length of 16
 * OUTPUT: rotated string
 * */
char* generate_access_token(char* clientIdToken) {
    char *token = malloc(TOKEN_LEN * sizeof(char*));
    int i, key, used[TOKEN_LEN];
    int rotationIndex = TOKEN_LEN;

    memset(used, 0, TOKEN_LEN * sizeof(int));
    for (i = 0; i < TOKEN_LEN; i++) {
        do {
            key = rand() % rotationIndex;
        } while (used[key] == 1);
        token[i] = clientIdToken[key];
        used[key] = 1;
    }
    token[TOKEN_LEN] = '\0';
    return token;
}
