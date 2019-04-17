/**
 * @file file.h
 *
 * @author Simon Petit
 */
#ifndef FILE_H
#define FILE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CONTENT_TYPE_SIZE 64
#define MAX_FORMAT_SIZE 8
#define NUMBER_FORMATS 25


/**
 * @brief Compute the string of the mime type accordingly to the format.
 *
 * @param[in] format The format of the file.
 *
 * @return The mime type standard for http responses.
 */
char *mime_type(char *format);

/**
 * @brief Compute the length of a file and set back the cursor to the beginning.
 *
 * @param[in] file An open file.
 *
 * @return The length of the file.
 */
int get_file_size(FILE *file);

/**
 * @brief Open any file and return its content as byte array
 *
 * @param[in] file
 * @param[in] length
 *
 * @return The content of the text file under char* form. Must be freed after
 * use.
 */
char *get_file_content(FILE *file, int length);

#endif
