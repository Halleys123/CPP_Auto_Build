#include "Logger.hpp"
#include "ConfigManager.hpp"

ConfigManager::ConfigManager(const char *configFile, Logger *logger)
{
    this->logger = logger;
    this->configFile = configFile;
    logger->log('I', "ConfigManager initiated with config file: %s%s%s", MEDIUM_STATE_BLUE_F, configFile, RESET_F);
}