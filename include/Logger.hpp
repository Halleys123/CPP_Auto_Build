#pragma once
#include <stdio.h>
#include <cstdarg>
#include "Colors.h"

class Logger
{
private:
    FILE *file;
    bool initialized;
    static int instanceCount;
    int instanceNumber;

    const char *getTime();
    void print(char status, const char *message, va_list args);
    void handleInstance();

public:
    Logger();
    Logger(const char *filename);
    ~Logger();
    void setLogFile(const char *filename);
    void log(char status, const char *message, ...);
};
