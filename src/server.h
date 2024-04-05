#ifndef SERVER_H
#define SERVER_H

#include "http.h"
#include <poll.h>

void *start_server(void *args);

void fetch_command(char *method, char *path, char *command);

void http_response(char *response, char *body);

#endif
