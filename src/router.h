#ifndef ROUTER_H
#define ROUTER_H

#include <stdio.h>

typedef struct route {
  char *path;
  char *method;
} route;

typedef struct router {
    int c;
    route routes[10];
} router;

#endif
