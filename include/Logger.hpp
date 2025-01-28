#pragma once
#include <stdio.h>
#include <cstdarg>
#include "Colors.h"

class Logger
{
private:
    FILE *file;
    bool initialized;
    bool shouldLogToFile = false;
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
    void enableLogging();
    void disableLogging();
};
