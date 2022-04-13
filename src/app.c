/**
 * @file app.c
 * @author Simon Petit
 * @brief 
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "app.h"

void printf_app(App *app)
{
  printf("Port : %d\n", app->port);
  printf("Connection file descriptor : %d\n", app->connfd);
}

int app_init(App *app, int port)
{
    app->connfd = 0;
    app->port = port;

    app->servin.sin_family = AF_INET;
    app->servin.sin_addr.s_addr = INADDR_ANY;
    app->servin.sin_port = htons(port);
    memset(app->servin.sin_zero, '\0', sizeof(app->servin.sin_zero));
    
    // app.router = (router){ .c = 0, .routes = {0} };
    
    if((app->serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    int reuse = 1;
    if (setsockopt(app->serverfd, SOL_SOCKET, SO_REUSEADDR, 
                (const char*)&reuse, sizeof(reuse)) < 0) {
        perror("In setsockopt");
        exit(EXIT_FAILURE);
    }


    if((bind(app->serverfd, (struct sockaddr *)&(app->servin), 
                    sizeof(struct sockaddr_in))) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }


    if((listen(app->serverfd, 10)) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    return 1;
}

void app_get(App *app, char *path)
{
    app->router.routes[app->router.c] = 
        (Route_t){ .path = path, .method = "GET"};
    app->router.c++;
}

void app_listen(App *app, int port)
{

    app_init(app, port);

    while(1) {
        int size = sizeof(app->servin);
        printf("\n+++++++ Waiting for new connection on port %d++++++++\n\n",
                app->port);
        app->connfd = accept(app->serverfd, 
                (struct sockaddr *)&(app->servin),(socklen_t*)&size);

        if (app->connfd < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        read(app->connfd, buffer, sizeof(buffer));
        printf("%s", buffer);
        int write_err = -1;

        // write_err = http_method(buffer, app->connfd);
        // printf("Characters send %d\n", write_err);

        close(app->connfd);

    }
}
