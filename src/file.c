/**
 * @file file.c
 * @author Simon Petit
 * @brief 
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "file.h"

char content_types[][MAX_CONTENT_TYPE_SIZE] = {
    "text/css",
    "text/csv",
    "text/html",
    "text/html",
    "image/gif",
    "image/x-icon",
    "image/jpeg",
    "image/jpeg",
    "image/png",
    "image/tiff",
    "image/tiff",
    "image/svg+xml",
    "image/webp",
    "application/javascript",
    "application/typescript",
    "application/json",
    "application/pdf",
    "application/x-tar",
    "application/xml",
    "application/zip",
    "application/java-archive",
    "font/otf",
    "font/ttf",
    "font/woff",
    "font/woff2"
};

char format_extensions[NUMBER_FORMATS][MAX_FORMAT_SIZE] = {
    "css",
    "csv",
    "html",
    "htm",
    "gif",
    "ico",
    "jpg",
    "jepg",
    "png",
    "tif",
    "tiff",
    "svg",
    "webp",
    "js",
    "ts",
    "json",
    "pdf",
    "tar",
    "xml",
    "zip",
    "jar",
    "otf",
    "ttf",
    "woff",
    "woff2"
};

void mime_type(char *format, char *content_type)
{
    unsigned int i;

    if (format == NULL) {
        strcpy(content_type, "text/html");
    } else {
        for (i = 0u; i < NUMBER_FORMATS; i++) {
            if (strcmp(format, format_extensions[i]) == 0) {
                strcpy(content_type, content_types[i]);
            }
        }
    }

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

