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
#include "router.h"

/**
 * @brief description of the web app.
 */
typedef struct app {
    int port;
    int connfd;
    int serverfd;
    struct sockaddr_in servin;
    struct sockaddr_in cliin;
    Router_t router;
} App;

/**
 * @brief Print the characteristics of the app.
 *
 * @param[in] app The app struct.
 */
void printf_app(App *app);

/**
 * @brief       initiate the app object.
 *
 * @param[in]   port    The port number where the app will be listening.
 *
 * @return      The app struct.
 */
int app_init(App *app, int port);

void app_get(App *app, char *path);

/**
 * @brief       Start the server, which make the app listen to the given port.
 *
 * @param[in]   app     A pointer to an allocated app structure.
 * @param[in]   port    The port number where the app will be listening.
 */
void app_listen(App *app, int port);

#endif
