/**
	Sisteme de programe pentru retele de calculatoare

	Copyright (C) 2008 Ciprian Dobre & Florin Pop
	Univerity Politehnica of Bucharest, Romania

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 */

#include <stdio.h>
#include <time.h>
#include <rpc/rpc.h>

#include "T1.h"
#include "token.h"

struct user_info
{
	char user_id[USER_ID_LEN+1];

	int has_refresh;
	char request_token[TOKEN_LEN+1];

	char access_token[TOKEN_LEN+1];
	char refresh_token[TOKEN_LEN+1];
	int remaining_life;
	// next element in list
	struct user_info *next;
} *users = NULL;

// searches for a user with that id and creates a new entry if it doesn't exist
struct user_info *get_user_info(char *user_id)
{
	struct user_info **info_addr = &users;

	for(; info_addr[0]; info_addr = &(info_addr[0]->next))
		if(!strcmp(info_addr[0]->user_id, user_id))
			return *info_addr;

	info_addr[0] = calloc(1, sizeof(struct user_info));
	strcpy(info_addr[0]->user_id, user_id);

	return *info_addr;

}

// performs token refresh
void refresh(struct user_info *current_user, CLIENT *handle)
{
	request_access_token_request args;
	args.is_refresh_request = 1;
	args.automatic_refresh = current_user->has_refresh;
	args.user_id = current_user->user_id;
	args.request_token = current_user->refresh_token;

	request_access_token_response *resp = request_access_token_1(&args, handle);

	current_user->remaining_life = resp->token_lifetime;
	strcpy(current_user->access_token, resp->access_token);
	strcpy(current_user->refresh_token, resp->refresh_token);
}


int main(int argc, char *argv[]){

	/* variabila clientului */
	CLIENT *handle;

	handle = clnt_create(
		argv[1],		/* numele masinii unde se afla server-ul */
		T1_PROG,		/* numele programului disponibil pe server */
		T1_VERS,		/* versiunea programului */
		"tcp");			/* tipul conexiunii client-server */

	if(handle == NULL) {
		perror("");
		return -1;
	}

	char *line = NULL, *user_id = NULL, *action;
	size_t line_max_len = 0;
	FILE *input_fd = fopen(argv[2], "r");

	struct user_info *current_user;

	while(getline(&line, &line_max_len, input_fd)>0)
	{
		if(line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = 0;

		user_id = strtok(line, ",");
		action = strtok(NULL, ",");

		current_user = get_user_info(user_id);

		if(!strcmp(action, "REQUEST"))
		{
			request_authorization_response *res = request_authorization_1(&user_id, handle);

			if(res->result_code == REQUEST_AUTHORIZATION_USER_NOT_FOUND)
			{
				puts("USER_NOT_FOUND");
				continue;
			}

			strcpy(current_user->request_token, res->request_token);

			char *request_token = strdup(current_user->request_token);
			approve_request_token_response *res2 = approve_request_token_1(&(request_token), handle);

			if(!res2->is_signed)
			{
				puts("REQUEST_DENIED");
				continue;
			}

			request_access_token_request args;
			args.is_refresh_request = 0;
			current_user->has_refresh = args.automatic_refresh = strtok(NULL, ",")[0] == '1';
			args.user_id = user_id;
			args.request_token = current_user->request_token;

			request_access_token_response *resp3 = request_access_token_1(&args, handle);

			current_user->remaining_life = resp3->token_lifetime;
			strcpy(current_user->access_token, resp3->access_token);
			if(args.automatic_refresh)
				strcpy(current_user->refresh_token, resp3->refresh_token);

			if(args.automatic_refresh)
				printf("%s -> %s,%s\n", args.request_token, resp3->access_token, resp3->refresh_token);
			else
				printf("%s -> %s\n", args.request_token, resp3->access_token);
		}
		else
		{
			validate_delegated_action_request param;
			param.target_resource = strtok(NULL, ",");
			param.access_token = current_user->access_token;
			param.action = action;

			if(current_user->has_refresh && !current_user->remaining_life)
				refresh(current_user, handle);

			int *res = validate_delegated_action_1(&param, handle);

			puts(validate_delegated_action_result_code_repr[*res]);
			current_user->remaining_life--;
		}
	}

	return 0;
}
