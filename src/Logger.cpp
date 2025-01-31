#include <stdio.h>
#include <chrono>
#include <cstdarg>
#include <regex>
#include <fstream> // Include ofstream
#include "ColorsDefinitions.h"
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

std::ofstream logFile; // Use ofstream instead of FILE*

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
    if (!isFailPrinted && shouldLogToFile && !initialized)
    {
        this->log('E', "Configuration file not set or found, set log file in configuration file");
        isFailPrinted = true;
        return;
    }
    if (shouldLogToFile && initialized && logFile.is_open())
    {
        // QUESTION(ARNAV): How to remove color related info from the message before writing to file?
        // ANSWER(STATUS): Complete by use of regex
        va_list args_copy;
        va_copy(args_copy, args);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), message, args_copy);
        std::string filteredMessage = buffer;
        filteredMessage = std::regex_replace(filteredMessage, std::regex("\x1b\\[[0-9;]*m"), "");
        logFile << getTime() << " " << statusFull << ": " << filteredMessage << std::endl;
        va_end(args_copy);
        logFile.flush(); // Ensure the log messages are written to the file immediately
    }
}

void Logger::handleInstance()
{
    instanceCount++;
    instanceNumber = instanceCount;
    log('I', "Logger instance count: %s%d%s", MEDIUM_STATE_BLUE_F, instanceCount, RESET_F);
}

Logger::Logger()
{
    handleInstance();
    initialized = false;
    shouldLogToFile = false; // Ensure shouldLogToFile is initialized
    log('S', "Logger Initiated in console log mode only");
}

Logger::Logger(const char *filename)
{
    if (!filename)
    {
        handleInstance();
        initialized = false;
        shouldLogToFile = false; // Ensure shouldLogToFile is initialized
        log('S', "Logger Initiated in console log mode only");
        return;
    }
    handleInstance();
    logFile.open(filename, std::ios::app);
    if (logFile.is_open())
    {
        initialized = true;
        shouldLogToFile = true; // Ensure shouldLogToFile is set to true
        log('S', "Logger Initiated in file log mode");
        log('I', "File that will be used for logging is: %s%s%s", MEDIUM_STATE_BLUE_F, filename, RESET_F);
    }
    else
    {
        initialized = false;
        shouldLogToFile = false; // Ensure shouldLogToFile is set to false
        log('E', "Failed to open log file: %s", filename);
    }
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        log('I', "Logger is closing, closing log file");
        logFile.close();
        log('S', "Log file closed successfully");
    }
}

void Logger::setLogFile(const char *filename)
{
    log('I', "Setting log file to %s%s%s", MEDIUM_STATE_BLUE_F, filename, RESET_F);
    std::ofstream newLogFile;
    newLogFile.open(filename, std::ios::app);
    if (newLogFile.is_open())
    {
        if (logFile.is_open())
        {
            log('W', "Closing old log file");
            logFile.close();
        }
        logFile = std::move(newLogFile);
        initialized = true;
        shouldLogToFile = true; // Ensure shouldLogToFile is set to true
        log('S', "%s%s%s file successfully opened.", MEDIUM_STATE_BLUE_F, filename, RESET_F);
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

void Logger::enableLogging()
{
    this->log('S', "Logging is enabled.");
    this->shouldLogToFile = true;
}

void Logger::disableLogging()
{
    this->log('S', "Logging is disabled.");
    this->shouldLogToFile = false;
}

bool Logger::isInit()
{
    return initialized;
}
