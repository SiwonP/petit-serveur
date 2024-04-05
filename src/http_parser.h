/**
 * @file "http_parser.h"
 *
 * @author Simon Petit
 */

#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <stdlib.h>

enum method {
    GET,
    POST,
    PUT, 
    DELETE,
    METHODS
};

char *parse_path(char *buffer, char *path);

char *parse_method(char *buffer, char *method);

char *parse_version(char *buffer, char *version);

/**
 * @brief Reads a header name in a string. Stop criteria is when the read char is ':'.
 *
 * @param[in] buffer The string to read.
 * @param[in,out] name The destination where to copy the found header name.
 *
 * @return The pointer to the string once the header name is read.
 */
char *parse_header_name(char * buffer, char* name);

/**
 * @brief Reads a header value in a string. Stop criteria is when the read char is '\r\n'.
 *
 * @param[in] buffer The string to read.
 * @param[in,out] name The destination where to copy the found header value.
 *
 * @return The pointer to the string once the header value is read.
 */
char *parse_header_value(char *buffer, char *value);

/**
 * @brief Reads a header in a string, first the header name, then the header value
 *
 * @param[in] buffer The string to read.
 * @param[in,out] header_name The destination where to copy the found header name.
 * @param[in,out] header_value The destination where to copy the found header value.
 *
 * @return The pointer to the string once the header is read.
 */
char *parse_header(char *buffer, char *header_name, char *header_value);

int parse_headers(char *buffer, char **header_names, char **header_values);

/**
 * @brief According to a criteria function, return the substring which match the criteria at each char 
 *
 * @param[in] buffer The string to read.
 * @param[in,out] dst The destination where to copy where criteria is fullfiled
 * @param[in] criteria The function evaluated at each char. Function stops when criteria return false.
 *
 * @return The pointer to the string to read after criteria is not matched anymore.
 */
char *take_while(char *buffer, char *dst, char (*criteria)(char *buffer));

/**
 * @brief According to a criteria function, return the substring which match the criteria at each char 
 *
 * @param[in] buffer The string to read.
 * @param[in,out] dst The destination where to copy where criteria is fullfiled
 * @param[in] criteria The function evaluated at each char. Function stops when criteria return true.
 *
 * @return The pointer to the string to read after criteria is not matched anymore.
 */
char *take_until(char *buffer, char *dst, char (*criteria)(char *buffer));

/**
 * @brief According to a criteria function, ignore the char matching the criteria.  
 *
 * @param[in] buffer The string to read.
 * @param[in] criteria The function evaluated at each char. Function stops when criteria return false.
 *
 * @return The pointer to the string to read after criteria is not matched anymore.
 */
char *drop_while(char *buffer, char (*criteria)(char *buffer));

char *drop_n_times(char *buffer, int n, char (*criteria)(char *buffer));

/**
 * @brief Return whether the char pointed matches ' '
 * 
 * @param[in] buffer The pointer of the char to read.
 * 
 * @return 1 if matched else 0 
*/
char match_ws(char *buffer);

/**
 * @brief Return whether the char pointed matches ':'
 * 
 * @param[in] buffer The pointer of the char to read.
 * 
 * @return 1 if matched else 0 
*/
char match_colon(char *buffer);

/**
 * @brief Return whether the char pointed matches '\r' or '\n'
 * 
 * @param[in] buffer The pointer of the char to read.
 * 
 * @return 1 if matched else 0 
*/
char match_crlf(char *buffer);

#endif