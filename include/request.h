#ifndef REQUEST_H
#define REQUEST_H

#define SHORT_STRING 64

typedef enum HTTP_Method
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    METHOD_COUNT
} HTTP_Method_e;

char HTTPMethodToString[METHOD_COUNT][SHORT_STRING] = {
    "GET",
    "HEAD",
    "POST",
    "PUT",
    "DELETE",
    "CONNECT",
    "OPTIONS",
    "TRACE"
};


typedef struct Request
{
    HTTP_Method_e method;
    /* data */
} Request_t;


#endif