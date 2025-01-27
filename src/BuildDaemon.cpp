#include "Logger.hpp"
#include "ConfigManager.hpp"

class BuildDaemon
{
    Logger *logger = nullptr;
    ConfigManager *configManager = nullptr;

public:
    BuildDaemon(const char *configFile, const char *logFile);
};

BuildDaemon::BuildDaemon(const char *configFile, const char *logFile)
{
    logger = new Logger(logFile);
    if (!logger)
    {
        printf("Failed to create logger\n");
        return;
    }
    configManager = new ConfigManager(configFile, logger);
    logger->log('I', "Log file: %s%s%s", MEDIUM_STATE_BLUE_F, logFile, RESET_F);
    logger->log('I', "Config file: %s%s%s", MEDIUM_STATE_BLUE_F, configFile, RESET_F);
}