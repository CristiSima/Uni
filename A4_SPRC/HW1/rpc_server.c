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
#include <rpc/pmap_clnt.h>

#include "T1.h"
#include "token.h"

void t1_prog_1(struct svc_req *rqstp, register SVCXPRT *transp);

struct permissions_set
{
	int entries;
	// pair of resource and permission
	char **resources;
	char **permissions;

	// pointer to next element used to store the chain of permissions from approvals.db
	struct permissions_set *next;
} *permission_list, *permissions_granted;

int user_count;
struct user_info {
	char user_id[USER_ID_LEN+1];

	char request_token[USER_ID_LEN+1];
	int is_request_token_signed;
	struct permissions_set *active_token_permisions;

	char access_token[USER_ID_LEN+1];
	char refresh_token[USER_ID_LEN+1];
	int token_lifetime;
} *users_info;

int resource_count;
char **resources;

int base_token_lifetime;

void init(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	// pointer managed by getline
	char * line = NULL;
	size_t max_len = USER_ID_LEN;

	FILE *users_fd = fopen(argv[1], "r");
	fscanf(users_fd, "%d\n", &user_count);
	users_info = calloc(sizeof(*users_info), user_count);

	for(int i = 0; i<user_count; i++)
	{
		getline(&line, &max_len, users_fd);
		line[USER_ID_LEN] = 0;

		strcpy(users_info[i].user_id, line);
	}
	fclose(users_fd);

	// resources
	FILE *resources_fd = fopen(argv[2], "r");
	fscanf(resources_fd, "%d\n", &resource_count);
	resources = calloc(sizeof(*resources), resource_count);
	max_len = RESOURCE_MAX_LEN;

	for(int i = 0; i<resource_count; i++)
	{
		getline(&line, &max_len, resources_fd);
		if(line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = 0;

		resources[i] = strdup(line);
	}
	fclose(resources_fd);

	// approvals.db
	FILE *permissions_fd = fopen(argv[3], "r");
	// pointer to the next field of the last element or the start of the list
	struct permissions_set **last_permission_set = &permission_list;

	while(0<getline(&line, &max_len, permissions_fd))
	{
		if(line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = 0;

		*last_permission_set = malloc(sizeof(**last_permission_set));
		last_permission_set[0]->next = NULL;
		// get entries count
		int entries_count = 0;
		for(char *p = line; p[0]; p++)
			entries_count+= p[0] == ',';

		// no permissions
		if(entries_count == 1 && strcmp(line, "*,-") == 0)
		{
			last_permission_set[0]->entries = 0;
			last_permission_set = &(last_permission_set[0]->next);
			continue;
		}

		last_permission_set[0]->entries = entries_count = (entries_count>>1) + (entries_count&1);

		last_permission_set[0]->resources = calloc(entries_count, sizeof(char*));
		last_permission_set[0]->permissions = calloc(entries_count, sizeof(char*));
		// split
		char *temp=line;
		for(int j = 0; j<entries_count; j++)
		{
			last_permission_set[0]->resources[j] = strdup(strtok(temp, ","));
			last_permission_set[0]->permissions[j] = strdup(strtok(NULL, ","));
			// set to NULL to not restart strtok
			temp = NULL;
		}

		last_permission_set = &(last_permission_set[0]->next);
	}
	free(line);
	permissions_granted = permission_list;

	base_token_lifetime = atoi(argv[4]);
}

static int *int2intp(int n)
{
	static int res;
	res = n;
	return &res;
}


request_authorization_response *request_authorization_1_svc(char **user_id, struct svc_req *cl)
{
	static request_authorization_response response;
	int index;
	for(index = 0; index<user_count; index++)
		if(!strcmp(users_info[index].user_id, *user_id))
			break;

	printf("BEGIN %s AUTHZ\n", *user_id);

	if(index == user_count)
	{
		response.result_code = REQUEST_AUTHORIZATION_USER_NOT_FOUND;
		response.request_token = *user_id;
		return &response;
	}

	users_info[index].is_request_token_signed = 0;

	response.result_code = REQUEST_AUTHORIZATION_GRANTED;
	response.request_token = generate_access_token(*user_id);
	strcpy(users_info[index].request_token, response.request_token);
	return &response;

}

struct request_access_token_response * request_access_token_1_svc(struct request_access_token_request *request, struct svc_req *cl)
{
	static struct request_access_token_response resp;
	resp.access_token = NULL;
	resp.refresh_token = NULL;
	resp.token_lifetime = 0;


	int index;
	for(index = 0; index<user_count; index++)
		if(!strcmp(users_info[index].user_id, request->user_id))
			break;


	if(index == user_count)
	{
		resp.result_code = REQUEST_ACCESS_TOKEN_DENIED;
		return &resp;
	}

	if(request->is_refresh_request)
		if(strcmp(users_info[index].refresh_token, request->request_token))
		{
			resp.result_code = REQUEST_ACCESS_TOKEN_DENIED;
			return &resp;
		}
		else
		{
			users_info[index].token_lifetime = base_token_lifetime;
			printf("BEGIN %s AUTHZ REFRESH\n", request->user_id);
		}
	else
		if(strcmp(users_info[index].request_token, request->request_token) || !users_info[index].is_request_token_signed)
		{
			resp.result_code = REQUEST_ACCESS_TOKEN_DENIED;
			return &resp;
		}

	resp.access_token = generate_access_token(request->request_token);
	resp.result_code = REQUEST_ACCESS_TOKEN_GRANTED;
	resp.token_lifetime = users_info[index].token_lifetime;

	resp.refresh_token = "";
	if(request->automatic_refresh)
		resp.refresh_token = generate_access_token(resp.access_token);


	printf("  AccessToken = %s\n", resp.access_token);
	strcpy(users_info[index].access_token, resp.access_token);
	strcpy(users_info[index].refresh_token, resp.refresh_token);
	if(request->automatic_refresh)
		printf("  RefreshToken = %s\n", resp.refresh_token);

	return &resp;
}

int * validate_delegated_action_1_svc(struct validate_delegated_action_request *request, struct svc_req *cl)
{
	#define print_result(result) 		printf("%s (%s,%s,%s,%d)\n",	\
				result,													\
				request->action,										\
				request->target_resource,								\
				request->access_token,									\
				(index == user_count)?0: users_info[index].token_lifetime	\
			)
	int index = user_count;

	if(request->access_token[0] == 0)
	{
		print_result("DENY");
		return int2intp(VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED);
	}

	for(index = 0; index<user_count; index++)
		if(!strcmp(users_info[index].access_token, request->access_token))
			break;

	if(index == user_count)
	{
		print_result("DENY");
		return int2intp(VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED);
	}

	if(!users_info[index].token_lifetime)
	{
		request->access_token[0] = 0;
		print_result("DENY");
		return int2intp(VALIDATE_DELEGATED_ACTION_TOKEN_EXPIRED);

	}
	users_info[index].token_lifetime--;

	int index_resource;
	for(index_resource = 0; index_resource<resource_count; index_resource++)
		if(!strcmp(resources[index_resource], request->target_resource))
			break;

	if(index_resource == resource_count)
	{
		print_result("DENY");
		return int2intp(VALIDATE_DELEGATED_ACTION_RESOURCE_NOT_FOUND);
	}

	for(int i = 0; i<users_info[index].active_token_permisions->entries; i++)
		if(!strcmp(request->target_resource, users_info[index].active_token_permisions->resources[i]))
		{
			char *permissions_on_target = users_info[index].active_token_permisions->permissions[i];
			if(!strcmp(request->action, "READ") && strchr(permissions_on_target, 'R'))
			{
				print_result("PERMIT");
				return int2intp(VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED);
			}

			if(!strcmp(request->action, "INSERT") && strchr(permissions_on_target, 'I'))
			{
				print_result("PERMIT");
				return int2intp(VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED);
			}

			if(!strcmp(request->action, "MODIFY") && strchr(permissions_on_target, 'M'))
			{
				print_result("PERMIT");
				return int2intp(VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED);
			}

			if(!strcmp(request->action, "DELETE") && strchr(permissions_on_target, 'D'))
			{
				print_result("PERMIT");
				return int2intp(VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED);
			}

			if(!strcmp(request->action, "EXECUTE") && strchr(permissions_on_target, 'X'))
			{
				print_result("PERMIT");
				return int2intp(VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED);
			}

			break;
		}

	print_result("DENY");
	return int2intp(VALIDATE_DELEGATED_ACTION_OPERATION_NOT_PERMITTED);

	#undef print_result
}

struct approve_request_token_response * approve_request_token_1_svc(char **request_token, struct svc_req *cl)
{
	static struct approve_request_token_response res;
	int index;

	printf("  RequestToken = %s\n", *request_token);

	for(index = 0; index<user_count; index++)
		if(users_info[index].request_token && !strcmp(users_info[index].request_token, *request_token))
			break;

	// check valid request_token and permissions granthed
	if(index == user_count || permissions_granted->entries == 0)
	{
		res.request_token = strdup(*request_token);
		users_info[index].active_token_permisions = NULL;
		permissions_granted = permissions_granted->next;
		users_info[index].token_lifetime = 0;
		res.is_signed = 0;
		return &res;
	}

	users_info[index].active_token_permisions = permissions_granted;
	permissions_granted = permissions_granted->next;
	users_info[index].token_lifetime = base_token_lifetime;

	users_info[index].is_request_token_signed = 1;
	res.request_token = strdup(*request_token);
	res.is_signed = 1;

	return &res;
}


// copied from normal _svc
int main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (T1_PROG, T1_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, T1_PROG, T1_VERS, t1_prog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (T1_PROG, T1_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, T1_PROG, T1_VERS, t1_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (T1_PROG, T1_VERS, tcp).");
		exit(1);
	}

	init(argc, argv);
	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
