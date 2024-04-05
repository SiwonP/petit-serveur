/**
 * @file server.c
 *
 * @brief Main program
 *
 * @author Simon Petit
 *
 * Main program of the tiny server
 *
 * \mainpage Petit Serveur
 *
 * \section Introduction
 *
 * This program is a tiny server written in C. The original purpose of this http
 * server is to server files and to help designers of websites.
 */
#include <postgres.h>
#include "executor/spi.h"

#include "server.h"
#include "http_parser.h"

void fetch_command(char *method, char *path, char *command)
{
	// SPI_connect();
	char base_query[] = "select command from http_server.path p join http_server.server s on p.id_api = s.id where p.path = '%s' and method = '%s'";
	char query[4096] = {0};
	sprintf(query, base_query, path, method);
	strncpy(command, query, strlen(query));
	// SPI_finish();
}


void *start_server(void *args)
{
	// int port = *(int*)args;
	int port = (int)args;
	// int port = 8080;

	struct pollfd pfds[1];
	// Accepting standard input to break the loop
	// pfds[0].fd = 0;
	// pfds[0].events = POLLIN;

	int serverfd = 0;
	int connfd = 0;
	struct sockaddr_in servin;
	struct sockaddr_in cliin;

	servin.sin_family = AF_INET;
	servin.sin_port = htons(port);
	servin.sin_addr.s_addr = INADDR_ANY;
	memset(servin.sin_zero, '\0', sizeof servin.sin_zero);

	if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	if ((bind(serverfd, (struct sockaddr *)&servin, sizeof(servin))) < 0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	if ((listen(serverfd, 10)) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	// Accepting the http socket to handle requests
	pfds[0].fd = serverfd;
	pfds[0].events = POLLIN;

	while (1)
	{
		char method[16] = {0};
		char version[16] = {0};
		char path[128] = {0};
		char buffer[4096] = {0};
		char header_name[16][128] = {0};
		char header_value[16][128] = {0};
		// char response[] = "HTTP/1.1 200 Ok\r\nContent-Length: 22\r\n\r\nSalut les petits potes";
		int write_err = 0;
		int i = 0;
		// pointer of the char of the ongoing parsing
		char *buf;
		
		char command[2048] = {0};
		char response[4096] = {0};

		int poll_count = poll(pfds, 1, -1);

		if (poll_count == -1)
		{
			perror("poll");
			exit(1);
		}

		if (poll_count == 0)
		{
			printf("Poll timed out!\n");
		}
		else
		{

			if (pfds[0].revents & POLLIN)
			{
				// 	break;
				// }
				// if (pfds[1].revents & POLLIN)
				// {
				int size = sizeof(servin);
				// printf("\n+++++++ Waiting for new connection ++++++++\n\n");
				connfd = accept(serverfd, (struct sockaddr *)&servin, (socklen_t *)&size);
				if (connfd < 0)
				{
					perror("In accept");
					exit(EXIT_FAILURE);
				}

				read(connfd, buffer, sizeof(buffer));

				write_err = -1;

				buf = parse_method(buffer, method);
				buf = drop_while(buf, &match_ws);
				buf = parse_path(buf, path);
				buf = drop_while(buf, &match_ws);
				buf = parse_version(buf, version);
				buf = drop_while(buf, &match_crlf);

				i = 0;
				// since we drop "\r\n" after the header we only need to check
				// another "\r\n" once
				while (*(buf) != '\r' || *(buf + 1) != '\n')
				{
					// Parse the header field
					buf = parse_header(buf, header_name[i], header_value[i]);
					// Drop "\r\n" once after the header
					buf = drop_n_times(buf, 2, &match_crlf);
					i++;
					// 	elog(INFO, "name: %s", header_name[i]);
					// 	elog(INFO, "value: %s", header_value[i]);
				}

				// fetch_command(method, path, command);
				strcpy(command, "salut les copains");
				http_response(response, command);

				// elog(INFO, "method: %s", method);
				// elog(INFO, "path: %s", path);

				write(connfd, response, strlen(response));

				close(connfd);
			}
		}
	}

	// return 0;
}

void http_response(char *response, char *body)
{
	char content_length = strlen(body);
	char content_length_header[64] = {0};
	sprintf(content_length_header, "Content-Length: %d\r\n\r\n", content_length);
	strcpy(response, "HTTP/1.1 200 OK\r\n");
	strcat(response, content_length_header);
	strcat(response, body);
}