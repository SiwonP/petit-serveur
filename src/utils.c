#include "utils.h"

void fetch_command(char *method, char *path, char *command)
{
	// SPI_connect();
	char base_query[] = "select command from http_server.path p join http_server.server s on p.id_api = s.id where p.path = '%s' and method = '%s'";
	char query[4096] = {0};
	sprintf(query, base_query, path, method);
	strncpy(command, query, strlen(query));
	// SPI_finish();
}
