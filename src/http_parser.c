/**
 * @file "http_parser.c"
 *
 * @author Simon Petit
 */

#include <stdio.h>
#include <string.h>

#include "http_parser.h"

long parse_ws(char *buffer)
{
   return *buffer == ' ' ? (long)(buffer+1) : (long)-1 ;
}

long parse_cr(char *buffer)
{
	return *buffer == '\r' ? (long)(buffer+1) : (long)-1;
}

long parse_lf(char *buffer)
{
	return *buffer == '\n' ? (long)(buffer+1) : (long)-1;
}

char *parse_method(char *buffer, char *method) 
{

	char methods[METHODS][32] = {
		"GET",
		"PUT",
		"POST",
		"DELETE"
	};
	char *result = 0;

	for (int i = 0; i < METHODS; i++) {
		strncpy(method, buffer, strlen(methods[i]));
		if (strcmp(method, methods[i]) == 0) {
			result = buffer + strlen(methods[i]);
			break;
		} else {
			memset(method, '\0', strlen(method));
		}
	}

	return result;
}

char *parse_path(char *buffer, char *path) {
	int i = 0;
	while (*(buffer+i) != ' ') {
		i++;
	}
	strncpy(path, buffer, i);
	return buffer+i;
}

char *parse_version(char *buffer, char *version) {
	char http[3][32] = {
		"HTTP/1.0",
		"HTTP/1.1",
		"HTTP/2"
	};
	char *result = 0;
	for (int i = 0; i < 3; i++) {
		strncpy(version, buffer, strlen(http[i]));
		if (strcmp(version, http[i]) == 0) {
			result = buffer + strlen(http[i]);
			break;
		} else {
			memset(version, '\0', strlen(version));
		}
	}

	return result;
}

char *parse_header_name(char * buffer, char* name)
{
	memset(name, '\0', strlen(name));
	return take_until(buffer, name, &match_colon);
}

char *parse_header_value(char *buffer, char *value)
{
	memset(value, '\0', strlen(value));
	char *buf = drop_while(buffer, &match_ws);
	buf = take_until(buf, value, &match_crlf);
	return buf;
}

char *parse_header(char *buffer, char *header_name, char *header_value)
{
	char *buf;

	buf = parse_header_name(buffer, header_name );
	buf = parse_header_value(buf + 1, header_value );

	return buf;
}

int parse_headers(char *buffer, char *header_names[], char *header_values[])
{
	int i = 0;
	char *buf;
	buf = parse_header_value(buffer, header_names[0]);
	// buf = parse_header(buffer, header_names[i], header_values[i]);

	return i;
}

char *take_while(char *buffer, char *dst, char (*criteria)(char *buffer))
{
	int i = 0;
	while( (*criteria)(buffer + i) ) {
		i++;
	}
	strncpy(dst, buffer, i);
	return buffer + i;
}

char *take_until(char *buffer, char *dst, char (*criteria)(char *buffer))
{
	int i = 0;
	while ( !(*criteria)(buffer+i) ) {
		i++;
	}
	strncpy(dst, buffer, i);
	return buffer + i;
}

char *drop_while(char *buffer, char (*criteria)(char *buffer))
{
	int i = 0;
	while( (*criteria)(buffer + i) ) {
		i++;
	}
	// strncpy(dst, buffer+i, strlen(buffer) - i);
	return buffer + i;
}

char match_colon(char *buffer)
{
	return *buffer == ':' ? 1 : 0;
}

char match_ws(char *buffer)
{
	return *buffer == ' ' ? 1 : 0;
}

char match_crlf(char *buffer)
{
	return (*buffer == '\r' || *buffer == '\n') ? 1 : 0;
}