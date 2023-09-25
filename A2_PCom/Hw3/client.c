#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

#define server_connection() open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0)
#define take_after(string, delim) (strstr((string), (delim))+strlen(delim))
#define fgets_no_nl(buff, max_len, file) fgets(buff, max_len, file); buff[strlen(buff)-1]=0;

#define MAX_COOKIE_COUNT 10
#define HEADER_TERMINATOR "\r\n\r\n"


int cookie_count=0;
char *cookies[MAX_COOKIE_COUNT];
char *authorization=NULL;

// wrapper for http requests
char *http_requeste(char *request)
{
    static int sockfd=0;
    if(sockfd==0)
        sockfd=server_connection();

    char *response;

    do
    {
        if(sockfd==0)
            sockfd=server_connection();

        send_to_server(sockfd, request);
    	response=receive_from_server(sockfd);

        if(response==NULL || response[0]==0)
        {
            close_connection(sockfd);
            sockfd=0;

            if(response!=NULL)
            {
                free(response);
                response=NULL;
            }
        }
    }
    while(response==NULL);

    return response;
}

void handle_register()
{
    // read user input
    char *username=calloc(1,LINELEN);
    printf("username=");
    fgets_no_nl(username, LINELEN, stdin);

    char *password=calloc(1,LINELEN);
    printf("password=");
    fgets_no_nl(password, LINELEN, stdin);

    char *response;
    char *message;

    // create json object for request
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);
    serialized_string = json_serialize_to_string_pretty(root_value);

    // build request
    char *args[]={ serialized_string};
    message=compute_post_request("34.241.4.235", "/api/v1/tema/auth/register",
                                "application/json", args, 1,
                                cookies, cookie_count, authorization);

    response=http_requeste(message);
	// puts(message);
	// puts(response);

    int response_code;
    sscanf(response, "HTTP/1.1 %d", &response_code);
    // printf("response_code: %d\n", response_code);

    // free json
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);

    // 201 CREATED
    if(response_code==201)
    {
        puts("User registered.");
        goto frees;
    }

    // 429 Too Many Requests
    if(response_code==429)
    {
        // puts("Wait a ");
        printf("Too many requests, wait %ss\n", strtok(take_after(response, "Retry-After: "), "\n\r"));
        goto frees;
    }

    // construct response json
    char *response_payload=take_after(response, HEADER_TERMINATOR);
    JSON_Value *response_json=json_parse_string(response_payload);

    // show error
    puts(json_object_get_string(json_object(response_json), "error"));

    // free response json
    json_value_free(response_json);

    frees:
    free(username);
    free(password);

	free(message);
    free(response);
}

void handle_login()
{
    char *username=calloc(1,LINELEN);
    char *password=calloc(1,LINELEN);

    // read user input
    printf("username=");
    fgets_no_nl(username, LINELEN, stdin);

    printf("password=");
    fgets_no_nl(password, LINELEN, stdin);

    char *response;
    char *message;

    // create json object for request
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char *serialized_string = json_serialize_to_string_pretty(root_value);

    // build request
    char *args[]={ serialized_string};
    message=compute_post_request("34.241.4.235", "/api/v1/tema/auth/login",
                                "application/json", args, 1,
                                cookies, cookie_count, authorization);

    response=http_requeste(message);
	// puts(message);
	// puts(response);

    int response_code;
    sscanf(response, "HTTP/1.1 %d", &response_code);
    // printf("response_code: %d\n", response_code);

    // free json
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);

    // 200 OK
    if(response_code==200)
    {
        // save session cookie
        char *cookie=strtok(take_after(response, "Set-Cookie: "), ";");
        cookies[cookie_count]=strdup(cookie);
        cookie_count++;

        puts("Logged in.");
        goto frees;
    }

    // 429 Too Many Requests
    if(response_code==429)
    {
        // puts("Wait a ");
        printf("Too many requests, wait %ss\n", strtok(take_after(response, "Retry-After: "), "\n\r"));
        goto frees;
    }

    // 204 No Content
    // returned when already logged in
    if(response_code==204)
    {
        puts("Already logged in.");
        goto frees;
    }

    // construct response json
    char *response_payload=take_after(response, HEADER_TERMINATOR);
    JSON_Value *response_json=json_parse_string(response_payload);

    // show error
    puts(json_object_get_string(json_object(response_json), "error"));

    // free response json
    json_value_free(response_json);

    frees:
    free(username);
    free(password);

	free(message);
    free(response);
}

void handle_enter_library()
{
    // int sockfd=server_connection();
    char *response;
    char *message;
    message=compute_get_request("34.241.4.235", "/api/v1/tema/library/access",
                                 NULL,
                                 cookies, cookie_count, authorization);

    response=http_requeste(message);
	// puts(message);
	// puts(response);

    // exception
    int response_code;
    sscanf(response, "HTTP/1.1 %d", &response_code);


    char *response_payload=take_after(response, HEADER_TERMINATOR);
    // puts(response_payload);

    // construct response json
    JSON_Value *response_json=json_parse_string(response_payload);

    printf("%d\n", response_code);

    if(response_code==401)
    {
        puts(json_object_get_string(json_object(response_json), "error"));
        goto frees;
    }

    // clear old authorization
    if(authorization)
        free(authorization);

    // save new authorization
    authorization=strdup(json_object_get_string(json_object(response_json), "token"));

    puts("Authorized.");

    frees:
    // free response json
    json_value_free(response_json);

	free(message);
    free(response);
}

void handle_get_books()
{
    char *response;
    char *message;

    // build request
    message=compute_get_request("34.241.4.235", "/api/v1/tema/library/books",
                                NULL,
                                cookies, cookie_count, authorization);

    response=http_requeste(message);
    // puts(message);
    // puts(response);

    int response_code;
    sscanf(response, "HTTP/1.1 %d", &response_code);

    char *response_payload=take_after(response, HEADER_TERMINATOR);

    // create json object from response
    JSON_Value *response_json=json_parse_string(response_payload);

    if(response_code==403)
    {
        puts(json_object_get_string(json_object(response_json), "error"));
        goto frees;
    }


    JSON_Array *books_array=json_value_get_array(response_json);

    // print a table of books
    puts("Id\tTitle");
    for(int i=0;i<json_array_get_count(books_array);i++)
    {
        JSON_Object *book_json=json_array_get_object(books_array, i);
        printf("%d\t%s\n", (int)json_object_get_number(book_json, "id"),
                                json_object_get_string(book_json, "title"));
    }


    frees:
    // free response json
    json_value_free(response_json);

	free(message);
    free(response);
}

void handle_add_book()
{
    char *response;
    char *message;

    // read user input
    char *title=calloc(1, LINELEN);
    printf("title=");
    fgets_no_nl(title, LINELEN, stdin);

    char *author=calloc(1, LINELEN);
    printf("author=");
    fgets_no_nl(author, LINELEN, stdin);

    char *genre=calloc(1, LINELEN);
    printf("genre=");
    fgets_no_nl(genre, LINELEN, stdin);

    char *publisher=calloc(1, LINELEN);
    printf("publisher=");
    fgets_no_nl(publisher, LINELEN, stdin);

    int page_count;
    char *page_count_line=calloc(1, LINELEN);
    printf("page_count=");
    fgets_no_nl(page_count_line, LINELEN, stdin);
    // extract and check
    if(!sscanf(page_count_line, "%d", &page_count))
    {
        puts("page_count must be a number!");
        goto frees_invalid_page_count;
    }

    // create json object for request
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;
    json_object_set_string(root_object, "title",     title);
    json_object_set_string(root_object, "author",    author);
    json_object_set_string(root_object, "genre",     genre);
    json_object_set_string(root_object, "publisher", publisher);
    json_object_set_number(root_object, "page_count",(double)page_count);
    serialized_string = json_serialize_to_string_pretty(root_value);

    // build request
    char *args[]={ serialized_string};
    message=compute_post_request("34.241.4.235", "/api/v1/tema/library/books",
                                "application/json", args, 1,
                                cookies, cookie_count, authorization);

    response=http_requeste(message);
	// puts(message);
	// puts(response);

    int response_code;
    sscanf(response, "HTTP/1.1 %d", &response_code);

    // 429 Too Many Requests
    if(response_code==429)
    {
        printf("Too many requests, wait %ss\n", strtok(take_after(response, "Retry-After: "), "\n\r"));
        goto frees;
    }

    // 403 Forbiden
    if(response_code==403)
    {
        char *response_payload=take_after(response, HEADER_TERMINATOR);
        JSON_Value *response_json=json_parse_string(response_payload);

        puts(json_object_get_string(json_object(response_json), "error"));

        json_value_free(response_json);
        goto frees;
    }

    puts("Book added");


    frees:
    // free json
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);

	free(message);
    free(response);

    frees_invalid_page_count:
    free(page_count_line);
    free(publisher);
    free(genre);
    free(author);
    free(title);
}

void handle_get_book()
{
    char *response;
    char *message;
    char url[LINELEN];

    // read user input
    int book_id;
    char *book_id_line=calloc(1, LINELEN);
    printf("id=");
    fgets_no_nl(book_id_line, LINELEN, stdin);
    sscanf(book_id_line, "%d", &book_id);

    // build request
    sprintf(url, "/api/v1/tema/library/books/%d", book_id);
    message=compute_get_request("34.241.4.235", url,
                                NULL,
                                cookies, cookie_count, authorization);

    response=http_requeste(message);
    // puts(message);
    // puts(response);

    int response_code;
    sscanf(response, "HTTP/1.1 %d", &response_code);

    // 429 Too Many Requests
    if(response_code==429)
    {
        printf("Too many requests, wait %ss\n", strtok(take_after(response, "Retry-After: "), "\n\r"));
        goto frees;
    }

    // construct response json
    char *response_payload=take_after(response, HEADER_TERMINATOR);
    JSON_Value *response_json=json_parse_string(response_payload);

    if(response_code==403 || response_code==404)
    {
        puts(json_object_get_string(json_object(response_json), "error"));
        goto frees;
    }

    // get the element
    // [book_json]
    JSON_Object *book_json=json_array_get_object(json_value_get_array(response_json), 0);

    // show book info
    printf("Title\t\t%s\nAuthor\t\t%s\nGenre\t\t%s\nPublisher\t%s\nPage count\t%d\n",
            json_object_get_string(book_json, "title"),
            json_object_get_string(book_json, "author"),
            json_object_get_string(book_json, "genre"),
            json_object_get_string(book_json, "publisher"),
            (int)json_object_get_number(book_json, "page_count"));

    frees:
    json_value_free(response_json);

	free(message);
    free(response);

    free(book_id_line);
}

void handle_delete_book()
{
    char *response;
    char *message;
    char url[LINELEN];

    int book_id;
    char *book_id_line=calloc(1, LINELEN);
    printf("id=");
    fgets_no_nl(book_id_line, LINELEN, stdin);
    sscanf(book_id_line, "%d", &book_id);

    sprintf(url, "/api/v1/tema/library/books/%d", book_id);
    message=compute_delete_request("34.241.4.235", url,
                                    NULL,
                                    cookies, cookie_count, authorization);

    response=http_requeste(message);
    // puts(message);
    // puts(response);

    int response_code;
    sscanf(response, "HTTP/1.1 %d", &response_code);

    // 429 Too Many Requests
    if(response_code==429)
    {
        printf("Too many requests, wait %ss\n", strtok(take_after(response, "Retry-After: "), "\n\r"));
        goto frees;
    }

    // construct response json
    char *response_payload=take_after(response, HEADER_TERMINATOR);
    JSON_Value *response_json=json_parse_string(response_payload);

    // 404 Not Found
    // 403 Forbiden
    if(response_code==404 || response_code==403)
    {
        puts(json_object_get_string(json_object(response_json), "error"));
        goto frees;
    }

    puts("Deleted.");

    frees:
    // free response json
    json_value_free(response_json);

	free(message);
    free(response);

    free(book_id_line);
}

void handle_logout()
{
    char *response;
    char *message;

    message=compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout",
                                NULL,
                                cookies, cookie_count, authorization);

    response=http_requeste(message);
    // puts(message);
    // puts(response);

    int response_code;
    sscanf(response, "HTTP/1.1 %d", &response_code);

    // 429 Too Many Requests
    if(response_code==429)
    {
        printf("Too many requests, wait %ss\n", strtok(take_after(response, "Retry-After: "), "\n\r"));
        goto frees;
    }

    // construct response json
    char *response_payload=take_after(response, HEADER_TERMINATOR);
    JSON_Value *response_json=json_parse_string(response_payload);

    if(response_code==404)
    {
        puts(json_object_get_string(json_object(response_json), "error"));
        goto frees;
    }

    // clear authorization
    if(authorization)
        free(authorization);
    authorization=NULL;

    // clear cookie
    if(cookie_count)
    {
        cookie_count--;
        free(cookies[cookie_count]);
        cookies[cookie_count]=NULL;
    }


    frees:
    json_value_free(response_json);

	free(message);
    free(response);
}

int main()
{
    char *command=calloc(1,LINELEN);


    while (1)
    {
        // command indicator
        printf("> ");

        fgets_no_nl(command, LINELEN, stdin);

        if(!strcmp(command, "register"))
            handle_register();
        else if(!strcmp(command, "login"))
            handle_login();
        else if(!strcmp(command, "enter_library"))
            handle_enter_library();
        else if(!strcmp(command, "get_books"))
            handle_get_books();
        else if(!strcmp(command, "get_book"))
            handle_get_book();
        else if(!strcmp(command, "add_book"))
            handle_add_book();
        else if(!strcmp(command, "delete_book"))
            handle_delete_book();
        else if(!strcmp(command, "logout"))
            handle_logout();
        else if(!strcmp(command, "exit"))
            break;
        else
            puts("unknow command");

        // command separator
        puts("");
        puts("");
    }

    free(command);

    // clear authorization
    if(authorization)
    {
        free(authorization);
        authorization=NULL;
    }

    // clear cookies
    for(int i=0;i<cookie_count;i++)
        free(cookies[i]);

    return 0;
}
