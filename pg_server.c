#include <postgres.h>
#include <fmgr.h>
#include "executor/spi.h"

#include <utils/numeric.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

// void start_accept(int port)
// {
// 	int serverfd = 0;
// 	int connfd = 0;
// 	int size;
// 	struct sockaddr_in servin;
// 	// struct sockaddr_in cliin;
// 	char buffer[4096] = {0};
// 	char response[] = "HTTP/1.1 200 Ok\r\n";

// 	servin.sin_family = AF_INET;
// 	servin.sin_port = htons(port);
// 	servin.sin_addr.s_addr = INADDR_ANY;
// 	memset(servin.sin_zero, '\0', sizeof servin.sin_zero);

// 	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
// 		perror("In socket");
// 		exit(EXIT_FAILURE);
// 	}

// 	if((bind(serverfd, (struct sockaddr *)&servin, sizeof(servin))) < 0) {
// 		perror("In bind");
// 		exit(EXIT_FAILURE);
// 	}

// 	if((listen(serverfd, 10)) < 0) {
// 		perror("In listen");
// 		exit(EXIT_FAILURE);
// 	}

// 	size = sizeof(servin);
// 	connfd = accept(serverfd, (struct sockaddr *)&servin,(socklen_t*)&size);
	
// 	read(connfd, buffer, sizeof(buffer));

// 	write(connfd, response, strlen(response));

// 	close(connfd);
    
// }

PG_FUNCTION_INFO_V1(start);

Datum start(PG_FUNCTION_ARGS) {

	int port = 8080;

	int serverfd = 0;
	int connfd = 0;
	int size;
	struct sockaddr_in servin;
	// struct sockaddr_in cliin;
	char buffer[4096] = {0};
	char response[] = "HTTP/1.1 200 Ok\r\n";

	int e;

	int spiconn;
	char query[] = "select port from http_server.server;";
	int query_result;

	servin.sin_family = AF_INET;
	servin.sin_port = htons(port);
	servin.sin_addr.s_addr = INADDR_ANY;
	memset(servin.sin_zero, '\0', sizeof servin.sin_zero);

	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	if((bind(serverfd, (struct sockaddr *)&servin, sizeof(servin))) < 0) {
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	if((listen(serverfd, 10)) < 0) {
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	size = sizeof(servin);
	connfd = accept(serverfd, (struct sockaddr *)&servin,(socklen_t*)&size);
	
	e = read(connfd, buffer, sizeof(buffer));

	e = write(connfd, response, strlen(response));
	if(e <= 0) {
		perror("In writing");
		exit(EXIT_FAILURE);
	}

	close(connfd);

	spiconn = SPI_connect();
	if (spiconn != SPI_OK_CONNECT) {
		perror("In SPI connect");
	}

	query_result = SPI_execute(query, true, 0);
	if(query_result == SPI_OK_SELECT) {
        TupleDesc tupdesc = SPI_tuptable->tupdesc;
        SPITupleTable *tuptable = SPI_tuptable;
		int j = 0;

		for (j = 0; j < tuptable->numvals; j++) {
			
		}
		
	} else {
		perror("In SPI_execute");

	}
	
	SPI_finish();
	PG_RETURN_INT32(1);

}
