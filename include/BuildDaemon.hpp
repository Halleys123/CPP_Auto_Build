#include "Logger.hpp"
#include "ConfigManager.hpp"

class BuildDaemon
{
    Logger *logger = nullptr;
    ConfigManager *configManager = nullptr;

public:
    BuildDaemon(const char *configFile, const char *logFile);
    void watch();
};
