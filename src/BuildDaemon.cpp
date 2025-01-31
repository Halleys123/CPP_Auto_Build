#include <chrono>
#include <thread>
#include <unordered_map>
#include <filesystem>
#include <string>
#include <cstring>
#include "Logger.hpp"
#include "ConfigManager.hpp"
#include "BuildDaemon.hpp"

using namespace std;
namespace fs = filesystem;

BuildDaemon::BuildDaemon(const char *configFile, const char *logFile)
{
    logger = new Logger(logFile);
    if (!logger)
    {
        printf("Failed to create logger\n");
        return;
    }
    configManager = new ConfigManager(configFile, logFile, logger);
    if (!configManager)
    {
        logger->log('E', "Failed to create ConfigManager");
        return;
    }
}
void BuildDaemon::watch()
{
    logger->log('W', "Started watch process...");
    const char *scanDir = configManager->getScanDirectory();
    logger->log('I', "Scanning directory: %s%s%s", MEDIUM_STATE_BLUE_F, scanDir, RESET_F);

    if (!fs::exists(fs::u8path(scanDir)))
    {
        logger->log('E', "Scan directory does not exist: %s", scanDir);
        return;
    }

    unordered_map<string, fs::file_time_type> lastModifiedTimes;

    // Initialize the map with the current modification times
    for (const auto &entry : fs::recursive_directory_iterator(fs::u8path(scanDir)))
    {
        if (should_check_for_extension(configManager->getExtensionsToCheck(), entry.path().extension().string().c_str()))
        {
            logger->log('I', "Found file: %s%s%s, Saving modify time as: ", MEDIUM_STATE_BLUE_F, entry.path().string().c_str(), RESET_F, get_current_time().c_str());
            lastModifiedTimes[entry.path().string()] = fs::last_write_time(entry);
        }
    }
    int time = configManager->getInterval();
    logger->log('I', "Checking for file modifications every %d milliseconds", time);
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(time));

        for (const auto &entry : fs::recursive_directory_iterator(fs::u8path(scanDir)))
        {
            if (should_check_for_extension(configManager->getExtensionsToCheck(), entry.path().extension().string().c_str()))
            {
                auto currentModifyTime = fs::last_write_time(entry);

                if (lastModifiedTimes.find(entry.path().string()) == lastModifiedTimes.end() ||
                    lastModifiedTimes[entry.path().string()] < currentModifyTime)
                {
                    logger->log('I', "File modified: %s%s%s", MEDIUM_STATE_BLUE_F, entry.path().string().c_str(), RESET_F);
                    lastModifiedTimes[entry.path().string()] = currentModifyTime;
                }
            }
        }
    }
}

bool BuildDaemon::should_check_for_extension(const char **extensions, const char *extension)
{
    if (!extensions)
    {
        return true;
    }
    for (int i = 0; extensions[i] != nullptr; i++)
    {
        if (strcmp(extensions[i], extension) == 0)
        {
            return true;
        }
    }
    return false;
}

string BuildDaemon::get_current_time()
{
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now_time));
    return string(buf);
}

unordered_map<string, time_t> BuildDaemon::get_files_last_modify_time(const char *path)
{
    unordered_map<string, time_t> file_mod_times;
    for (const auto &entry : fs::recursive_directory_iterator(fs::u8path(path)))
    {
        if (entry.path().extension() == ".cpp" && entry.path().filename() != "auto_update.cpp")
        {
            file_mod_times[entry.path().string()] = fs::last_write_time(entry).time_since_epoch().count();
        }
    }
    return file_mod_times;
}
