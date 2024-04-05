#ifndef PG_SERVER_H
#define PG_SERVER_H

#include <postgres.h>
#include <fmgr.h>
#include "executor/spi.h"
#include "funcapi.h"

#include "utils/builtins.h"
#include <utils/numeric.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "src/server.h"

/**
 * 
*/
void update_state(int port, int state);

/**
 * 
*/
void get_ports(int **ports, int *counter);

/**
 * 
*/
Datum start_http_server(PG_FUNCTION_ARGS);

/**
 * 
*/
Datum start(PG_FUNCTION_ARGS);

#endif