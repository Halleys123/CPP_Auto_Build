#include "Logger.hpp"
#include "ConfigManager.hpp"
#include "BuildDaemon.hpp"

BuildDaemon::BuildDaemon(const char *configFile, const char *logFile)
{
    logger = new Logger(logFile);
    if (!logger)
    {
        printf("Failed to create logger\n");
        return;
    }
    configManager = new ConfigManager(configFile, logger);
    if (!configManager)
    {
        logger->log('E', "Failed to create ConfigManager");
        return;
    }
}

void BuildDaemon::watch()
{
}