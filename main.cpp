#include "ColorsDefinitions.h"
#include "BuildDaemon.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void parsePathsFromArguments(const char *&configFile, const char *&logFile, char **argv, int argc)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            configFile = argv[i + 1];
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            logFile = argv[i + 1];
        }
    }
}

int main(int argc, char **argv)
{
    const char *configFile = nullptr;
    const char *logFile = nullptr;
    parsePathsFromArguments(configFile, logFile, argv, argc);

    // TODO(Major): Configmanager is currently using string and vector in some places that should be replaced
    // TODO: with const char * and char ** respectively to reduce size and reduce dependencies
    // TODO(BUG): Currently ConfigManager is using c_str for many of the state variables which is causing
    // TODO: state variables to become dangling pointers replace them with strcpy method
    BuildDaemon buildDaemon(configFile, logFile);
    buildDaemon.watch();
    return 0;
}