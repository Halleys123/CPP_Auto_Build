#pragma once
#include "Logger.hpp"

class ConfigManager
{
private:
    Logger *logger = nullptr;
    const char *configFile = nullptr;

    int Interval = NULL;
    bool LogToFile = NULL;
    const char *LogFile = nullptr;
    const char *ScanDirectory = nullptr;
    const char **ExcludeDirectories = nullptr;
    const char **ExcludeFiles = nullptr;
    const char **ExtensionsToCheck = nullptr;
    const char **SpecialBuildCommands = nullptr;

private:
    void ParseFile(int parseCount = 0);
    bool CreateFile();

public:
    ConfigManager(const char *configFile, const char *logFile, Logger *logger);

    int getInterval();
    bool getLogToFile();
    const char *getLogFile();
    const char *getScanDirectory();
    const char **getExcludeDirectories();
    const char **getExcludeFiles();
    const char **getExtensionsToCheck();
    const char **getSpecialBuildCommands();
};