#include <string>
#include <unordered_map>
#include "Logger.hpp"
#include "ConfigManager.hpp"

class BuildDaemon
{
    Logger *logger = nullptr;
    ConfigManager *configManager = nullptr;
    std::string get_current_time();
    std::unordered_map<std::string, time_t> get_files_last_modify_time(const char *path);
    bool should_check_for_extension(const char **extensions, const char *extension);

public:
    BuildDaemon(const char *configFile, const char *logFile);
    void watch();
};
