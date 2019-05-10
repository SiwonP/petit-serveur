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
#include "server.h"
#include "app.h"

void test(int (*fn)(const char *oui, ...), char *string)
{
  (*fn)(string);
}

/**
 * @brief Launch the server.
 */
int main(int argc, char const *argv[])
{
    int port = 8080;

    test(&printf, "test\n");

    app app = init_app(port);
    app_listen(app);

    return 0;
}
