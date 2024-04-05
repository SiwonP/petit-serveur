#include "test.h"

int main()
{
    plan(3);

    char *buf;
    char header[] = "Content-Length: 11\r\n";
    char name[64] = {0};
    char expected_name[] = "Content-Length";
    char value[64] = {0};
    char expected_value[] = "11";

    buf = parse_header(&header[0], name, value);

    ok(strcmp(name, expected_name) == 0, "matching header name");
    ok(strcmp(value, expected_value) == 0, "matching header value");

    char command[256] = {0};
    char expected_command[] = "select command from http_server.path p join http_server.server s on p.id_api = s.id where p.path = '/' and method = 'GET'";

    fetch_command("GET", "/", command);

    ok(strcmp(command, expected_command) == 0);
}