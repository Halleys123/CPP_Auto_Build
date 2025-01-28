#pragma once
#include "Logger.hpp"

class ConfigManager
{
private:
    Logger *logger = nullptr;
    const char *configFile = nullptr;

    int Interval = NULL;
    bool LogToFile = NULL;
    const char **ExcludeDirectories = nullptr;
    const char **ExcludeFiles = nullptr;
    const char **ExtensionsToCheck = nullptr;

private:
    void ParseFile(int parseCount = 0);
    void CreateFile();

public:
    ConfigManager(const char *configFile, Logger *logger);

    int getInterval();
    bool getLogToFile();
    const char **getExcludeDirectories();
    const char **getExcludeFiles();
    const char **getExtensionsToCheck();
};