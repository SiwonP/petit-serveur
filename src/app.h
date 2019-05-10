#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "http.h"

typedef struct app {
  int port;
  int connfd;
  int serverfd;
  struct sockaddr_in servin;
  struct sockaddr_in cliin;
} app;

void printf_app(app app);

app init_app(int port);

void app_listen(app app);

#endif
