#include "app.h"

void printf_app(App *app)
{
  printf("Port : %d\n", app->port);
  printf("Connection file descriptor : %d\n", app->connfd);
}

App *init_app(int port)
{
    App *app;
    app = malloc(sizeof(struct app));
    app->port = port;
    app->connfd = 0;

    struct sockaddr_in *servin, *cliin;
    servin = malloc(sizeof(struct sockaddr_in));
    cliin = malloc(sizeof(struct sockaddr_in));

    servin->sin_family = AF_INET;
    servin->sin_port = htons(port);
    servin->sin_addr.s_addr = INADDR_ANY;

    app->servin = servin;
    app->cliin = cliin;

    memset(servin->sin_zero, '\0', sizeof(servin->sin_zero));
    printf("%d\n", port);
    /*
       app.router = (router){ .c = 0, .routes = {0} };


*/
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


    if((bind(app->serverfd, (struct sockaddr *)servin, 
                    sizeof(struct sockaddr_in))) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }


    if((listen(app->serverfd, 10)) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    free(servin);
    free(cliin);

    return app;

}

void app_get(App *app, char *path)
{
    app->router.routes[app->router.c] = 
        (route){ .path = path, .method = "GET"};
    app->router.c++;
}

void app_listen(App *app)
{
    while(1) {
        int size = sizeof(app->servin);
        printf("\n+++++++ Waiting for new connection on port %d++++++++\n\n",
                app->port);
        app->connfd = accept(app->serverfd, 
                (struct sockaddr *)app->servin,(socklen_t*)&size);

        if (app->connfd < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        read(app->connfd, buffer, sizeof(buffer));

        int write_err = -1;

        write_err = http_method(buffer, app->connfd);
        printf("Characters send %d\n", write_err);

        close(app->connfd);

    }
}
