#ifndef ROUTER_H
#define ROUTER_H

#include <stdio.h>
/**
 * @brief 
 * 
 */
typedef struct Route {
  char *path;
  char *method;
} Route_t;

/**
 * @brief 
 * 
 */
typedef struct Router {
    int c;
    Route_t *routes;
} Router_t;

#endif