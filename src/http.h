#ifndef HTTP_H
#define HTTP_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "file.h"

struct http_response {
    int code;
	char *version;
	char **headers_name;
	char **headers_value;
	char *body;
};

struct http_request {
	char *method;
	char *version;
	char *path;
	char **params;
	char **params_value;
	char **headers_name;
	char **headers_value;
	char *body;
};


/**
 * @brief Construct the header of the http response.
 *
 * @param[in] code The http code.
 * @param[in] content_type The type of the content, according to http standard.
 * @param[in] content_length the length of the content to be send.
 *
 * return The string containing the header.
 */
char *http_header(int code, char *content_type,
        int content_length);

/**
 * @brief Construct the code string containing the integer code and the 
 * corresponding string.
 *
 * @param[in] code The http code.
 *
 * return The concatenation of the code and its translation.
 */
char *http_code(int code);

/**
 * @brief Read the file from a get request and return the char array containg
 * the file.
 *
 * @param[in] path The path of the file requested.
 * @param[in] fd The file descriptor of the socket where to send the response.
 *
 * @return The result from the write function on the socket fd.
 */
int http_get(char *path, char *mime, int fd);

/**
 * @brief Construct the the http response from the http request.
 *
 * @param[in] request The raw request from the client.
 * @param[in] fd The file description of the socket where to send the response.
 *
 * @return The appropriate http response corresponding to the request.
 */
int http_method(char *request, int fd);

#endif
