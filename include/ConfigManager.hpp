#include "Logger.hpp"

class ConfigManager
{
private:
    Logger *logger = nullptr;
    const char *configFile = nullptr;

public:
    ConfigManager(const char *configFile, Logger *logger);
};