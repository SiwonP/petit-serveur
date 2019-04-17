/**
 * @file "file.c"
 *
 * @author Simon Petit
 */
#include "file.h"

char content_types[][MAX_CONTENT_TYPE_SIZE] = {
    "text/css",
    "text/csv",
    "text/html",
    "image/gif",
    "image/x-icon",
    "image/jpeg",
    "image/png",
    "image/tiff",
    "application/javascript",
    "application/json",
    "application/pdf"
};

char format_extensions[NUMBER_FORMATS][MAX_FORMAT_SIZE] = {
    "css",
    "csv",
    "html",
    "gif",
    "ico",
    "jpg",
    "png",
    "tif",
    "js",
    "json",
    "pdf"
};

char *mime_type(char *format)
{
    int i;
    char *content_type;
    if (format == NULL) {
        return "text/html";
    } else {
        for (i = 0; i < NUMBER_FORMATS; i++) {
            if (strcmp(format, format_extensions[i]) == 0) {
                content_type = content_types[i];
            }
        }
    }
    return content_type;
}


int get_file_size(FILE *file)
{
    int seek, length = 0;
    if (file != NULL) {
        seek = fseek(file, 0, SEEK_END);
        length = ftell(file);
        rewind(file);
    }
    return length;
}

char *get_file_content(FILE *file, int length)
{
    char *content;
    length = get_file_size(file);
    if (file == NULL) {
        content = NULL;
    } else {
        content = (char *)calloc(length, sizeof(char));
        fread(content, sizeof(char), length+1, file);
    }
    return content;
}

