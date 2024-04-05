#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>

enum {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    LEVELS
};

const char *levels[] = ["TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"];

void log(level_e level, char *fmt, ...)

#endif