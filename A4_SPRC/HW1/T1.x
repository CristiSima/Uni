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
#define USER_ID_LEN 15
#define TOKEN_LEN 15
#define RESOURCE_MAX_LEN 100
#define ACTION_MAX_LEN 100

struct request_authorization_response
{
	int result_code;
	string request_token<USER_ID_LEN>;
};

struct request_access_token_request
{
	int automatic_refresh;
	int is_refresh_request;
	string user_id<USER_ID_LEN>;
	string request_token<TOKEN_LEN>;
};

struct request_access_token_response
{
	int result_code;
	string access_token<TOKEN_LEN>;
	string refresh_token<TOKEN_LEN>;
	int token_lifetime;
};

struct validate_delegated_action_request
{
	string action<ACTION_MAX_LEN>;
	string target_resource<RESOURCE_MAX_LEN>;
	string access_token<TOKEN_LEN>;
};

struct approve_request_token_response
{
	string request_token<TOKEN_LEN>;
	unsigned int is_signed;
};

program T1_PROG {
	version T1_VERS {
		/* Server Authorization */
		struct request_authorization_response request_authorization(string<USER_ID_LEN>) = 1;
		struct request_access_token_response request_access_token(struct request_access_token_request) = 2;

		/* Server Resources */
		int validate_delegated_action(struct validate_delegated_action_request) = 3;

		/* User interaction */
		struct approve_request_token_response approve_request_token(string<USER_ID_LEN>) = 4;
	} = 1;
} = 123456789;
