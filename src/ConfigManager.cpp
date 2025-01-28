#include "Logger.hpp"
#include "Colors.h"
#include "ConfigManager.hpp"
#include <fstream>
#include <stdio.h>

#define DEFAULT_FILE "config.cfg"

ConfigManager::ConfigManager(const char *configFile, Logger *logger)
{
    this->logger = logger;
    this->configFile = configFile;
    logger->log('I', "ConfigManager initiated with config file: %s%s%s", MEDIUM_STATE_BLUE_F, configFile, RESET_F);
    this->ParseFile();
}

void ConfigManager::ParseFile(int parseTries = 0)
{
    if (!configFile)
    {
        logger->log('W', "No config file found, using default values.");
        // create file
        // write to that file with default values.
        CreateFile();
        // try reparsing.
        if (parseTries)
            ParseFile(parseTries - 1);
        return;
    }
    logger->log('I', "Parsing values from the file %s%s%s", MEDIUM_STATE_BLUE_F, this->configFile, RESET_F);
    // std::fstream ConfigFile(this->configFile);
    std::ifstream ConfigFile(this->configFile);
    if (!ConfigFile)
    {
        logger->log('W', "File can't be opened hence can't be parsed");
        logger->log('E', "Exiting build daemon.");
        exit(1);
    }
    // std::string line;
    std::string line;
    while (std::getline(ConfigFile, line))
    {
        // TODO(ARNAV): Add check for wether only one = is present checking position of first and last =
        int pos = line.find("=");
        if (line[0] == '#')
        {
            logger->log('I', "Comment detected, ignoring line.");
        }
        if (pos == std::string::npos)
        {
            logger->log('W', "Invalid line in config file: %s", line.c_str());
            continue;
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1, line.size());

        if (key == "INTERVAL")
        {
            this->Interval = std::stoi(value);
        }
        else if (key == "LOGTOFILE")
        {
            this->LogToFile = (value == "1");
        }
        // else if (key == "EXCLUDEDIRECTORIES")
        // {
        //     // Assuming directories are separated by commas
        //     this->ExcludeDirectories = split(value, ',');
        // }
        // else if (key == "EXCLUDEFILES")
        // {
        //     // Assuming files are separated by commas
        //     this->ExcludeFiles = split(value, ',');
        // }
        // else if (key == "EXTENSIONSTOCHECK")
        // {
        //     // Assuming extensions are separated by commas
        //     this->ExtensionsToCheck = split(value, ',');
        // }
        else
        {
            logger->log('W', "Unknown key in config file: %s", key.c_str());
        }
    }
}
void ConfigManager::CreateFile()
{

    std::fstream ConfigFile(DEFAULT_FILE);
    ConfigFile << "INTERVAL=150\n";
    logger->log('I', "Writing to config file: %sINTERVAL=150%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "LOGTOFILE=0\n";
    logger->log('I', "Writing to config file: %sLOGTOFILE=1%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "EXCLUDEDIRECTORIES=./bin\n";
    logger->log('I', "Writing to config file: %sEXCLUDEDIRECTORIES=./bin%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "EXCLUDEFILES=\n";
    logger->log('I', "Writing to config file: %sEXCLUDEFILES=%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "EXTENSIONSTOCHECK=c,cpp,h,hpp\n";
    logger->log('I', "Writing to config file: %sEXTENSIONSTOCHECK=c,cpp,h,hpp%s", MEDIUM_STATE_BLUE_F, RESET_F);

    this->configFile = DEFAULT_FILE;
}

int ConfigManager::getInterval()
{
    return this->Interval;
}

bool ConfigManager::getLogToFile()
{
    return this->LogToFile;
}
const char **ConfigManager::getExcludeDirectories()
{
    return this->ExcludeDirectories;
}
const char **ConfigManager::getExcludeFiles()
{
    return this->ExcludeFiles;
}
const char **ConfigManager::getExtensionsToCheck()
{
    return this->ExtensionsToCheck;
}