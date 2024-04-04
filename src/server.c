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

#include "server.h"
#include "http_parser.h"


int start_server(int port)
{
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

	elog(WARNING, "ici");

	while (1)
	{

		int poll_count = poll(pfds, 1, -1);
		elog(WARNING, "ici");


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
				printf("\n+++++++ Waiting for new connection ++++++++\n\n");
				connfd = accept(serverfd, (struct sockaddr *)&servin, (socklen_t *)&size);
				if (connfd < 0)
				{
					perror("In accept");
					exit(EXIT_FAILURE);
				}

				char buffer[4096] = {0};
				read(connfd, buffer, sizeof(buffer));

				int write_err = -1;

				char method[16] = {0};
				char version[16] = {0};
				char path[128] = {0};

				char *buf = parse_method(buffer, method);
				buf = parse_ws(buf);

				buf = parse_path(buf, path);

				buf = parse_ws(buf);

				buf = parse_version(buf, version);

				buf = parse_cr(buf);

				buf = parse_lf(buf);

				char header_name[16][128] = {0};
				char header_value[16][128] = {0};

				int i = 0;
				while (*(buf) != '\r' || *(buf + 1) != '\n' || *(buf + 2) != '\r' || *(buf + 3) != '\n')
				{
					buf = parse_header(buf, header_name[i], header_value[i]);
					elog(WARNING, "%s\n", header_name[i]);
					elog(WARNING, "%s\n\n", header_value[i]);
				}

				elog(WARNING, "%s", method);
				elog(WARNING, "%s", path);

				char response[] = "HTTP/1.1 200 Ok\r\n";

				write(connfd, response, strlen(response));

				close(connfd);
			}
		}
	}

	return 0;
}

// int main(int argc, char const *argv[])
// {
// 	int port = 8080;
// 	start_server(port);
// 	return 0;
// }
