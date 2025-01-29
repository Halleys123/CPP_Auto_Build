#include "Logger.hpp"
#include "Colors.h"
#include "ConfigManager.hpp"
#include <fstream>
#include <stdio.h>

#define DEFAULT_FILE "config.cfg"

ConfigManager::ConfigManager(const char *configFile, const char *logFile, Logger *logger)
{
    int parseTries = 2;

    this->logger = logger;
    this->configFile = configFile;
    this->LogFile = logFile;

    logger->log('I', "ConfigManager initiated with config file: %s%s%s", MEDIUM_STATE_BLUE_F, configFile, RESET_F);
    this->ParseFile(parseTries);
}

void ConfigManager::ParseFile(int parseTries)
{
    // QUESTION(Halleys): What is happening in this if logic below
    // ANSWER(Halleys):

    std::ifstream ConfigFile;

    if (!configFile)
    {
        logger->log('W', "Configuration File not provided, trying default");
        logger->log('I', "Finding default configuration file: config.cfg");
        this->configFile = "config.cfg";
        ConfigFile.open(this->configFile);
        if (ConfigFile.is_open())
        {
            logger->log('S', "Success in loading default configuration file: %s%s%s", MEDIUM_STATE_BLUE_F, DEFAULT_FILE, RESET_F);
        }
    }
    else
    {
        ConfigFile.open(this->configFile);
    }

    if (!ConfigFile.is_open())
    {
        logger->log('E', "Failed to load default config.cfg file.");
        logger->log('W', "Creating configuration file with default values.");
        if (!CreateFile())
        {
            logger->log('E', "Failed to create config file.");
            exit(1);
        }
        logger->log('S', "Default configuration file created sucessfully.");
        this->configFile = DEFAULT_FILE;
        ConfigFile.open(this->configFile);
    }

    if (!ConfigFile.is_open())
    {
        logger->log('E', "Failed to open/create config file after retries.");
        exit(1);
    }
    // std::string line;
    logger->log('I', "Parsing values from the file %s%s%s", MEDIUM_STATE_BLUE_F, this->configFile, RESET_F);
    std::string line;
    int lineNumber = 0;
    while (std::getline(ConfigFile, line))
    {
        lineNumber += 1;
        // TODO(ARNAV): Add check for wether only one equalto (=) is present by checking position of first and last euqalto(=)
        int pos = line.find("=");
        if (line[0] == '#')
        {
            logger->log('W', "Comment detected, ignoring line number: %d", lineNumber);
            continue;
        }
        if (pos == std::string::npos)
        {
            logger->log('E', "Invalid line in config file: %s", line.c_str());
            continue;
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1, line.size());

        if (key == "INTERVAL")
        {
            this->Interval = std::stoi(value);
            logger->log('I', "Using Interval from configuration file: %s%d%s miliseconds", MEDIUM_STATE_BLUE_F, this->Interval, RESET_F);
        }
        else if (key == "LOGTOFILE")
        {
            this->LogToFile = (value == "1");
            logger->log('I', "Logs will %s%s logged%s to file", MEDIUM_STATE_BLUE_F, this->LogToFile ? "be" : "not be", RESET_F);

            if (this->LogFile)
                logger->setLogFile(this->LogFile);
        }
        else if (key == "LOGFILE")
        {
            // ? This makes sure if the user has provided logfile in the arguments of program then
            // ? it will not use value from the configuration file.
            if (value == "")
            {
                logger->log('W', "Logfile not provided in configuration file.");
                continue;
            }
            if (logger->isInit() && this->LogFile)
            {
                logger->log('W', "%sLOGFILE%s path provided in arguments, ignoring path in configuration file.", MEDIUM_STATE_BLUE_F, RESET_F);
                continue;
            }

            this->LogFile = value.c_str();
            if (this->LogToFile)
            {

                logger->log('I', "Using %s as the logging file.", value.c_str());
                logger->setLogFile(this->LogFile);
            }
            else
            {
                logger->log('I', "Logging is disabled, hence not using %s as log file.", value.c_str());
            }
        }
        else if (key == "EXCLUDEDIRECTORIES")
        {
            // INFO: p_ = parsed
            if (value == "")
            {
                logger->log('I', "No exclude directories found");
                this->ExcludeDirectories = new const char *[1];
                this->ExcludeDirectories[0] = nullptr;
                continue;
            }
            int commas = 0;
            std::string dir = "";
            for (char i : value)
            {
                if (i == ',')
                    commas += 1;
            }
            logger->log('I', "Found %d exclude directories: %s%s%s", commas + 1, MEDIUM_STATE_BLUE_F, value.c_str(), RESET_F);

            const char **p_ExcludeDirectories = new const char *[commas + 2];
            int pos = 0;
            for (char i : value)
            {
                if (i == ',')
                {
                    p_ExcludeDirectories[pos] = dir.c_str();
                    pos += 1;
                    dir = "";
                    continue;
                }
                dir += i;
            }
            p_ExcludeDirectories[pos] = dir.c_str();
            p_ExcludeDirectories[pos + 1] = nullptr;
            this->ExcludeDirectories = p_ExcludeDirectories;
        }
        else if (key == "EXCLUDEFILES")
        {
            printf("%s%s%s\n", YELLOW_F, value.c_str(), RESET_F);
            if (value == "")
            {
                logger->log('I', "No exclude files found");
                this->ExcludeFiles = new const char *[1];
                this->ExcludeFiles[0] = nullptr;
                continue;
            }
            int commas = 0;
            std::string file = "";
            for (char i : value)
            {
                if (i == ',')
                    commas += 1;
            }
            logger->log('I', "Found %d exclude files: %s%s%s", commas + 1, MEDIUM_STATE_BLUE_F, value.c_str(), RESET_F);

            const char **p_ExcludeFiles = new const char *[commas + 2];
            int pos = 0;
            for (char i : value)
            {
                if (i == ',')
                {
                    p_ExcludeFiles[pos] = file.c_str();
                    pos += 1;
                    file = "";
                    continue;
                }
                file += i;
            }
            p_ExcludeFiles[pos] = file.c_str();
            p_ExcludeFiles[pos + 1] = nullptr;
            this->ExcludeFiles = p_ExcludeFiles;
        }
        else if (key == "EXTENSIONSTOCHECK")
        {
            if (value == "")
            {
                logger->log('I', "No extensions to check found");
                this->ExtensionsToCheck = new const char *[1];
                this->ExtensionsToCheck[0] = nullptr;
                continue;
            }
            int commas = 0;
            std::string ext = "";
            for (char i : value)
            {
                if (i == ',')
                    commas += 1;
            }
            logger->log('I', "Found %d extensions to check: %s%s%s", commas + 1, MEDIUM_STATE_BLUE_F, value.c_str(), RESET_F);

            const char **p_ExtensionsToCheck = new const char *[commas + 2];
            int pos = 0;
            for (char i : value)
            {
                if (i == ',')
                {
                    p_ExtensionsToCheck[pos] = ext.c_str();
                    pos += 1;
                    ext = "";
                    continue;
                }
                ext += i;
            }
            p_ExtensionsToCheck[pos] = ext.c_str();
            p_ExtensionsToCheck[pos + 1] = nullptr;
            this->ExtensionsToCheck = p_ExtensionsToCheck;
        }
        else
        {
            // TODO(ARNAV): Check for file names seperated by comma and a make command after equal
            // ? If any of these file changes then build command specified in front of them will run
            // ? Example:
            // ? Logger.cpp,Logger.hpp=make dllcmd
            // ? test.h=make test
            // * Here only processing the files in a 2d table be done here
            // * Actual file checking should be done in BuildDaemon
            logger->log('W', "Unknown key in config file: %s", key.c_str());
        }
    }
}
bool ConfigManager::CreateFile()
{

    std::fstream ConfigFile(DEFAULT_FILE);
    ConfigFile << "INTERVAL=150\n";
    logger->log('I', "Writing to config file: %sINTERVAL=150%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "LOGTOFILE=0\n";
    logger->log('I', "Writing to config file: %sLOGTOFILE=1%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "LOGFILE=./auto_build.log\n";
    logger->log('I', "Writing to config file: %sLOGFILE=auto_build.log%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "EXCLUDEDIRECTORIES=./bin\n";
    logger->log('I', "Writing to config file: %sEXCLUDEDIRECTORIES=./bin%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "EXCLUDEFILES=\n";
    logger->log('I', "Writing to config file: %sEXCLUDEFILES=%s", MEDIUM_STATE_BLUE_F, RESET_F);
    ConfigFile << "EXTENSIONSTOCHECK=c,cpp,h,hpp\n";
    logger->log('I', "Writing to config file: %sEXTENSIONSTOCHECK=c,cpp,h,hpp%s", MEDIUM_STATE_BLUE_F, RESET_F);

    this->configFile = DEFAULT_FILE;
    return ConfigFile.is_open();
}

// Getter functions
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
const char *ConfigManager::getLogFile()
{
    return this->LogFile;
}