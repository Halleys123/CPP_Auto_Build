#include <stdio.h>
#include <chrono>
#include <cstdarg>
#include <regex>
#include "Colors.h"
#include "Logger.hpp"

// TEXT COLOR
const char *WARNING_TEXT_COLOR = ORANGE_F;
const char *SUCCESS_TEXT_COLOR = GREEN_F;
const char *INFO_TEXT_COLOR = BLUE_F;
const char *ERROR_TEXT_COLOR = RED_F;

struct STATUS
{
    char SUCCESS = 'S';
    char ERROR = 'E';
    char WARNING = 'W';
    char INFO = 'I';
};

int Logger::instanceCount = 0;

const char *Logger::getTime()
{
    // format = [HH-MM-SS::DD-MM-YYYY]
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char *time = (char *)malloc(20);
    sprintf(time, "[%02d:%02d:%02d::%02d-%02d-%d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    return time;
}

void Logger::print(char status, const char *message, va_list args)
{
    const char *color = nullptr;
    const char *statusFull = nullptr;
    switch (status)
    {
    case 'S':
        color = SUCCESS_TEXT_COLOR;
        statusFull = "SUCCESS";
        break;
    case 'E':
        color = ERROR_TEXT_COLOR;
        statusFull = "ERROR";
        break;
    case 'W':
        color = WARNING_TEXT_COLOR;
        statusFull = "WARNING";
        break;
    case 'I':
        color = INFO_TEXT_COLOR;
        statusFull = "INFO";
        break;
    default:
        color = GREY_F;
        statusFull = "UNKNOWN";
        break;
    }

    // Printing date and time, status and color
    printf("%s%s%s%s %s%s: %s", GREY_F, DATE_BACKGROUND, getTime(), RESET_BACKGROUND, color, statusFull, RESET_F);

    // Printing main message passed by user
    vprintf(message, args);

    // Printing new line and resetting color
    printf("%s\n", RESET_F);

    // Printing to file if initialized
    if (initialized && file)
    {
        // QUESTION(ARNAV): How to remove color related info from the message before writing to file?
        // ANSWER(STATUS): Complete by use of regex
        va_list args_copy;
        va_copy(args_copy, args);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), message, args_copy);
        std::string filteredMessage = buffer;
        filteredMessage = std::regex_replace(filteredMessage, std::regex("\x1b\\[[0-9;]*m"), "");
        fprintf(file, "%s %s: %s\n", getTime(), statusFull, filteredMessage.c_str());
        va_end(args_copy);
        fflush(file); // Ensure the log messages are written to the file immediately
    }
}

void Logger::handleInstance()
{
    instanceCount++;
    instanceNumber = instanceCount;
    log('I', "Logger instance count: %d", instanceCount);
}

Logger::Logger()
{
    handleInstance();
    initialized = false;
    file = nullptr;
    log('S', "Logger Initiated in console log mode only");
}

Logger::Logger(const char *filename)
{
    handleInstance();
    file = fopen(filename, "a+");
    if (file)
    {
        initialized = true;
        log('S', "Logger Initiated in file log mode");
        log('I', "File that will be used for logging is: %s", filename);
    }
    else
    {
        initialized = false;
        log('E', "Failed to open log file: %s", filename);
    }
}

Logger::~Logger()
{
    if (file)
    {
        log('I', "Logger is closing, closing log file");
        fclose(file);
        log('S', "Log file closed successfully");
    }
}

void Logger::setLogFile(const char *filename)
{
    FILE *newFile = fopen(filename, "a+");
    if (newFile)
    {
        if (file)
        {
            fclose(file);
        }
        file = newFile;
        initialized = true;
    }
    else
    {
        log('E', "Failed to set log file to: %s", filename);
    }
}

void Logger::log(char status, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    print(status, message, args);
    va_end(args);
}