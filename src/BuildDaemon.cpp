#include <chrono>
#include <thread>
#include <unordered_map>
#include <filesystem>
#include <string>
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
    logger->log('I', "Started watch process...");
    const char *scanDir = configManager->getScanDirectory();
    if (!fs::exists(fs::u8path(scanDir)))
    {
        logger->log('E', "Scan directory does not exist: %s", scanDir);
        return;
    }
    auto current_mod_times = get_files_last_modify_time(scanDir);
    auto last_mod_times = get_files_last_modify_time(scanDir);
    while (true)
    {
        this_thread::sleep_for(chrono::seconds(configManager->getInterval()));
        auto current_mod_times = get_files_last_modify_time(configManager->getScanDirectory());
        for (const auto &file : current_mod_times)
        {
            // if ExtensionsToCheck contains the extension of the file
            if (configManager->getExtensionsToCheck())
            {
                if (configManager->getExtensionsToCheck()[0] == nullptr)
                {
                    logger->log('I', "No extensions to check found");
                    break;
                }
                for (const char **ext = configManager->getExtensionsToCheck(); *ext; ext++)
                {
                    if (file.first.find(*ext) != string::npos)
                    {
                        if (last_mod_times[file.first] != current_mod_times[file.first])
                        {
                            logger->log('I', "File %s%s%s has been modified", MEDIUM_STATE_BLUE_F, file.first.c_str(), RESET_F);
                            last_mod_times[file.first] = current_mod_times[file.first];
                        }
                    }
                }
            }
        }
    }
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
