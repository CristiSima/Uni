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

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd=open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    // Ex 1.1: GET dummy from main server
	message=compute_get_request("34.241.4.235", "/api/v1/dummy", NULL, NULL, 0);
	send_to_server(sockfd, message);
	response=receive_from_server(sockfd);
	
	puts(response);
	free(message); free(response);

    // Ex 1.2: POST dummy and print response from main server
	char *args[]={ "as=ss"};
    message=compute_post_request("34.241.4.235", "/api/v1/dummy", "application/x-www-form-urlencoded", args, 1, NULL, 0);
	send_to_server(sockfd, message);
	response=receive_from_server(sockfd);
	puts(message);
	puts(response);

	// Ex 2: Login into main server
	char *auth_login[]={ "username=student", "password=student"};
    message=compute_post_request("34.241.4.235", "/api/v1/auth/login", "application/x-www-form-urlencoded", auth_login, 2, NULL, 0);
	send_to_server(sockfd, message);
	response=receive_from_server(sockfd);
	puts(message);
	puts(response);

	//close_connection(sockfd);
    //sockfd=open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
	
	// Ex 3: GET weather key from main server
	char *cookie[]={"connect.sid=s%3Aw7Ekiaak4r3KB52rA1SuSE3fIZ5m3gQo.Bf0cZ8BRTN6UJ4kxeZsrLOQQc9JpQI7nz6feH%2FUBwfo"};
    message=compute_get_request("34.241.4.235", "/api/v1/weather/key", NULL, cookie, 1);
	send_to_server(sockfd, message);
	response=receive_from_server(sockfd);
	puts(message);
	puts(response);


	// Ex 4: GET weather data from OpenWeather API
    // Ex 5: POST weather data for verification to main server
    // Ex 6: Logout from main server

    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!

    return 0;
}
