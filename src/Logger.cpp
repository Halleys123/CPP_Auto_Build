#include <stdio.h>
#include <chrono>
#include <cstdarg>
#include <cstdlib>

// FOREGROUND
const char *YELLOW_F = "\x1b[38;2;250;218;94m";
const char *RED_F = "\x1b[38;2;255;56;0m";
const char *WHITE_F = "\x1b[38;2;255;255;255m";
const char *GREY_F = "\x1b[38;2;192;192;192m";
const char *ORANGE_F = "\x1b[38;2;255;140;0m";
const char *DARK_GREY_F = "\x1b[38;2;155;155;155m";
const char *GREEN_F = "\x1b[38;2;63;255;0m";
const char *BLUE_F = "\x1b[38;2;0;185;232m";
const char *JOGGER_BLUE_F = "\x1b[38;2;138;185;241m";
const char *RESET_F = "\x1b[39m";

// BACKGROUND
const char *RESET_BACKGROUND = "\x1b[49m";
const char *DATE_BACKGROUND = "\x1b[48;2;15;7;0m";

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

class Logger
{
private:
    FILE *file;
    bool initialized = false;
    static int instanceCount;
    int instanceNumber = 0;

public:
    const char *red = RED_F;
    const char *yellow = YELLOW_F;
    const char *white = WHITE_F;
    const char *green = GREEN_F;
    const char *joggerBlue = JOGGER_BLUE_F;
    const char *reset = RESET_F;

private:
    const char *getTime()
    {
        // format = [HH-MM-SS::DD-MM-YYYY]
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char *time = (char *)malloc(20);
        sprintf(time, "[%02d:%02d:%02d::%02d-%02d-%d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        return time;
    }

    void print(char status, const char *message, va_list args)
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
            va_list args_copy;
            va_copy(args_copy, args);
            fprintf(file, "%s %s: ", getTime(), statusFull);
            vfprintf(file, message, args_copy);
            fprintf(file, "\n");
            va_end(args_copy);
        }
    }
    void handleInstance()
    {
        instanceCount++;
        instanceNumber = instanceCount;
        log('I', "Logger instance count: %d", instanceCount);
    }

public:
    Logger()
    {
        handleInstance();
        initialized = false;
        file = nullptr;
        log('S', "Logger Initiated in console log mode only");
    }

    Logger(const char *filename)
    {
        handleInstance();
        file = fopen(filename, "w");
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

    ~Logger()
    {
        if (file)
        {
            log('I', "Logger is closing, closing log file");
            fclose(file);
            log('S', "Log file closed successfully");
        }
    }

    void setLogFile(const char *filename)
    {
        FILE *newFile = fopen(filename, "w");
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

    void log(char status, const char *message, ...)
    {
        va_list args;
        va_start(args, message);
        print(status, message, args);
        va_end(args);
    }
};

int Logger::instanceCount = 0;