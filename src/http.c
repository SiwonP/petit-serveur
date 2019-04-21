/**
 * @file http.c
 *
 * @author Simon Petit
 */
#include "http.h"

char *http_header(int code, char *content_type,
        int content_length)
{
    time_t current_time = time(NULL);
    char *code_s;
    char *length_s = (char *)calloc(8, sizeof(char));
    char *date = (char *)calloc(32, sizeof(char));
    char *header = (char *)calloc(256, sizeof(char));

    header = strcat(header, "HTTP/1.1 ");
    code_s = http_code(200);
    header = strcat(header, code_s);
    date = strcat(date, "Date: ");
    date = strcat(date, ctime(&current_time));
    header = strcat(header, date);
    header = strcat(header, "Server: Petit Serveur\n");
    header = strcat(header, "Content-Type: ");
    header = strcat(header, content_type);
    header = strcat(header, "\nContent-Length: ");
    sprintf(length_s, "%d\n\n", content_length);
    header = strcat(header, length_s);

    free(code_s);
    free(length_s);
    free(date);
    return header;
}

char *http_code(int code)
{
    char *code_s = (char *)calloc(64, sizeof(char));
    sprintf(code_s, "%d ", code);
    if (code == 200) {
        code_s = strcat(code_s, "OK\n");
    } else if (code == 301) {
        code_s = strcat(code_s, "Move Permanently\n");
    } else if (code == 400) {
        code_s = strcat(code_s, "Bad Request\n");
    } else if (code == 403) {
        code_s = strcat(code_s, "Forbidden\n");
    } else if (code == 404) {
        code_s = strcat(code_s, "Not Found\n");
    } else if (code == 500) {
        code_s = strcat(code_s, "Internal Server Error\n");
    }
    return code_s;

}

int http_method(char *request, int fd)
{
    int length, write_err;
    char *method, *path, *content, *response, *format, *mime;
    method = strtok(request, " ");
    path = strtok(NULL, " ");
    char *filename = (char *)calloc(strlen(path), sizeof(char));
    strcpy(filename, path);

    /*
     * Get the format of the file so that the http header may be built
     * accordingly.
     */
    format = strtok(filename, ".");
    format = strtok(NULL, ".");
    /*
     * Extract the mime type out of the previous format.
     */
    mime = mime_type(format);
    printf("%s %s %s %s\n\n", method, path, format, mime);

    if (strcmp(method, "GET") == 0) {
         write_err = http_get(path, mime, fd);
    }
    return write_err;
}

int http_get(char *path, char *mime, int fd)
{
    int length = 0, write_err = 0;
    char *filename = (char *)calloc(strlen(path), sizeof(char));
    /*
     * Check the path requested is empty or not and return "index.html" by
     * default if yes.
     */
    if (strcmp(path, "/") == 0) {
        filename = (char *)realloc(filename, strlen("index.html")*sizeof(char));
        filename = strcpy(filename, "index.html");
    } else {
        /*
         * Passing path+1 as argument to ignore the initial "/"
         */
        filename = strcpy(filename, path+1);
    }

    FILE *file = fopen(filename, "r");

    length = get_file_size(file);

    char *content;
    char *header;
    if (length > 0) {
        content = get_file_content(file, length);
        header = http_header(200, mime, length);
    } else {
        content = (char *)calloc(strlen("NOT FOUND"), sizeof(char));
        content = strcpy(content, "NOT FOUND");
        length = strlen(content);
        header = http_header(404, "text/plain", length);
    }



    write_err = write(fd, header, strlen(header));
    write_err = write(fd, content, length);

    free(header);
    free(content);
    free(filename);
    fclose(file);
    return write_err;
}
