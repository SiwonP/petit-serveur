#include "app.h"

void printf_app(app app)
{
  printf("Port : %d\n", app.port);
  printf("Connection file descriptor : %d\n", app.connfd);
}

app init_app(int port)
{
  app app;
  app.port = port;
  app.connfd = 0;
  app.servin.sin_family = AF_INET;
  app.servin.sin_port = htons(port);
  app.servin.sin_addr.s_addr = INADDR_ANY;

  memset(app.servin.sin_zero, '\0', sizeof app.servin.sin_zero);

  if((app.serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("In socket");
      exit(EXIT_FAILURE);
  }

  if((bind(app.serverfd, (struct sockaddr *)&app.servin, sizeof(app.servin))) < 0) {
      perror("In bind");
      exit(EXIT_FAILURE);
  }

  if((listen(app.serverfd, 10)) < 0) {
      perror("In listen");
      exit(EXIT_FAILURE);
  }

  return app;
}

void app_get(app app)
{
  
}

void app_listen(app app)
{
  while(1) {
      int size = sizeof(app.servin);
      printf("\n+++++++ Waiting for new connection ++++++++\n\n");
      app.connfd = accept(app.serverfd, (struct sockaddr *)&app.servin,(socklen_t*)&size);

      if (app.connfd < 0) {
          perror("In accept");
          exit(EXIT_FAILURE);
      }

      char buffer[1024] = {0};
      read(app.connfd, buffer, sizeof(buffer));

      int write_err = -1;

      write_err = http_method(buffer, app.connfd);
      printf("Characters send %d\n", write_err);

      close(app.connfd);

  }
}
