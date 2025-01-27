#include "Logger.cpp"
#include <iostream>
#include <ostream>
#include <istream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdarg>

// Workflow of ConfigManager should be
// 1. Read from the config file
// 2. Parse those values if error use default values
// 3. If not present use default values

using namespace std;

// Default Values
static int dInterval = 200;
static bool dLogToFile = false;
static const char *dLogFilePath = "./";
static const char *dLogFileName = "log.txt";
static const char *dExcludeDirectories[] = {nullptr};
static const char *dExcludeFiles[] = {nullptr};
static const char *dExtensionToCheck[] = {"cpp", "hpp", "c", "h"};

// Same variables will be present in the config.cfg fiel but in capital letters
class ConfigManager
{
    // if some value is not present in the config file, the default value will be used
    // if the value is present but is empty or invalid, the default value will be used
    // if the value is present and is valid, the value from the config file will be used
private:
    int Interval;
    bool LogToFile;

    Logger *ConfigManagerLogger;

    const char *LogFilePath = nullptr;
    const char *LogFileName = nullptr;
    const char **excludeDirectories;
    const char **excludeFiles;
    const char **extensionToCheck;

    const char *configFilePath = "./config.cfg";
    FILE *configFile = nullptr;

private:
    void writeToFile(const char *key, const char *value)
    {
        // find the same key if it is present then replace the value
        // if not present then add the key value pair
        // if the key is invalid then log the error
        if (!configFile)
        {
            ConfigManagerLogger->log('E', "Config file not opened");
            return;
        }
        const char *tempKey;
        while (fscanf(configFile, "%s", tempKey) != EOF)
        {
            if (strcmp(tempKey, key) == 0)
            {
                fprintf(configFile, "%s=%s\n", key, value);
                return;
            }
        }

        delete tempKey;
    }
    void parseLine(const string &key, const string &value)
    {
        ConfigManagerLogger->log('I', "Parsing key: %s", key.c_str());
        if (key == "INTERVAL")
        {
            try
            {
                Interval = stoi(value);
                ConfigManagerLogger->log('I', "Interval set to: %s%d%s", ConfigManagerLogger->joggerBlue, Interval, ConfigManagerLogger->reset);
            }
            catch (...)
            {
                ConfigManagerLogger->log('E', "Invalid value for key: %s", key.c_str());
                Interval = dInterval;
            }
        }
        else if (key == "LOGTOFILE")
        {
            if (value != "true" && value != "false")
            {
                ConfigManagerLogger->log('E', "Invalid value for key: %s", key.c_str());
                ConfigManagerLogger->log('W', "Using default key fro LOGTOFILE: %s%s%s", ConfigManagerLogger->joggerBlue, dLogToFile ? "true" : "false", ConfigManagerLogger->reset);
                LogToFile = dLogToFile;
                return;
            }
            LogToFile = value == "true" ? true : false;
            ConfigManagerLogger->log('I', "Logging to Log file is: %s%s%s", ConfigManagerLogger->joggerBlue, LogToFile ? "enabled" : "disabled", ConfigManagerLogger->reset);
        }
        else if (key == "LOGFILENAME")
        {
            if (value.empty())
            {
                ConfigManagerLogger->log('E', "Invalid value for key: %s", key.c_str());
                ConfigManagerLogger->log('W', "Using default key fro LOGFILENAME: %s", dLogFileName);
                LogFileName = dLogFileName;
                return;
            }
            LogFileName = value.c_str();
            ConfigManagerLogger->log('I', "LogFileName set to: %s%s%s", ConfigManagerLogger->joggerBlue, LogFileName, ConfigManagerLogger->reset);
        }
        else if (key == "LOGFILEPATH")
        {
            if (value.empty())
            {
                ConfigManagerLogger->log('E', "Invalid value for key: %s", key.c_str());
                ConfigManagerLogger->log('W', "Using default key fro LOGFILEPATH: %s", dLogFilePath);
                LogFilePath = dLogFilePath;
                return;
            }
            LogFilePath = value.c_str();
            ConfigManagerLogger->log('I', "LogFilePath set to: %s%s%s", ConfigManagerLogger->joggerBlue, LogFilePath, ConfigManagerLogger->reset);
        }
        else if (key == "EXCLUDEDIRECTORIES")
        {
            // get values seperated by ,
            // if empty then use default value
            if (value.empty())
            {
                ConfigManagerLogger->log('W', "Using default key fro EXCLUDEDIRECTORIES: NULL");
                excludeDirectories = dExcludeDirectories;

                if (!dExcludeDirectories)
                    return;

                string tempValue = "";
                for (int i = 0; i < sizeof(dExcludeDirectories) / sizeof(dExcludeDirectories[0]); i++)
                {
                    tempValue += dExcludeDirectories[i];
                    if (i != sizeof(dExcludeDirectories) / sizeof(dExcludeDirectories[0]) - 1)
                        tempValue += ",";
                }

                writeToFile(key.c_str(), tempValue.c_str());
                return;
            }
            string temp = value;
            int count = 0;
            for (int i = 0; i < value.size(); i++)
            {
                if (value[i] == ',')
                    count++;
            }
            excludeDirectories = new const char *[count + 1];
            int j = 0;
            for (int i = 0; i < value.size(); i++)
            {
                if (value[i] == ',')
                {
                    excludeDirectories[j] = temp.substr(0, i).c_str();
                    temp = temp.substr(i + 1);
                    j++;
                }
            }
            // ConfigManagerLogger->log('I', "ExcludeDirectories set to: ");
            for (int i = 0; i < count; i++)
            {
                ConfigManagerLogger->log('I', "ExcludeDirectories[%d] set to: %s%s%s", i, ConfigManagerLogger->joggerBlue, excludeDirectories[i], ConfigManagerLogger->reset);
            }
        }
        else if (key == "EXCLUDEFILES")
        {
            // get values seperated by ,
            // if empty then use default value

            if (value.empty())
            {
                ConfigManagerLogger->log('I', "Using default key for EXCLUDEFILES: NULL");
                excludeFiles = dExcludeFiles;

                if (!dExcludeFiles)
                    return;

                string tempValue = "";
                for (int i = 0; i < sizeof(dExcludeFiles) / sizeof(dExcludeFiles[0]); i++)
                {
                    tempValue += dExcludeFiles[i];
                    if (i != sizeof(dExcludeFiles) / sizeof(dExcludeFiles[0]) - 1)
                        tempValue += ",";
                }

                writeToFile(key.c_str(), tempValue.c_str());
                return;
            }
            string temp = value;
            int count = 0;
            for (int i = 0; i < value.size(); i++)
            {
                if (value[i] == ',')
                    count++;
            }
            excludeFiles = new const char *[count + 1];
            int j = 0;
            for (int i = 0; i < value.size(); i++)
            {
                if (value[i] == ',')
                {
                    excludeFiles[j] = temp.substr(0, i).c_str();
                    temp = temp.substr(i + 1);
                    j++;
                }
            }
            for (int i = 0; i < count; i++)
            {
                ConfigManagerLogger->log('I', "ExcludeFiles[%d] set to: %s%s%s", i, ConfigManagerLogger->joggerBlue, excludeFiles[i], ConfigManagerLogger->reset);
            }
        }
        else if (key == "EXTENSIONTOCHECK")
        {
            // get values seperated by ,
            // if empty then use default value
            if (value.empty())
            {
                ConfigManagerLogger->log('E', "Invalid value for key: %s", key.c_str());
                ConfigManagerLogger->log('W', "Using default key fro EXTENSIONTOCHECK: %s", dExtensionToCheck[0]);
                extensionToCheck = dExtensionToCheck;
                // write to file seperated by , from teh array
                if (!dExtensionToCheck)
                    return;

                string tempValue = "";
                for (int i = 0; i < sizeof(dExtensionToCheck) / sizeof(dExtensionToCheck[0]); i++)
                {
                    tempValue += dExtensionToCheck[i];
                    if (i != sizeof(dExtensionToCheck) / sizeof(dExtensionToCheck[0]) - 1)
                        tempValue += ",";
                }
                writeToFile(key.c_str(), tempValue.c_str());
                return;
            }
            string temp = value;
            int count = 0;

            for (int i = 0; i < value.size(); i++)
            {
                if (value[i] == ',')
                    count++;
            }

            extensionToCheck = new const char *[count + 1];
            int j = 0;
            for (int i = 0; i < value.size(); i++)
            {
                if (value[i] == ',')
                {
                    extensionToCheck[j] = temp.substr(0, i).c_str();
                    temp = temp.substr(i + 1);
                    j++;
                }
            }
            ConfigManagerLogger->log('I', "ExtensionToCheck set to: ");
        }
        else
        {
            ConfigManagerLogger->log('E', "Invalid key: %s", key.c_str());
        }
    }

public:
    ConfigManager()
    {
        ConfigManagerLogger = new Logger();
    }
    ConfigManager(Logger &LogManager, const string &configFilePath = "./config.cfg") : ConfigManagerLogger(&LogManager), configFilePath(configFilePath.c_str())
    {
        ifstream configFile(configFilePath);
        if (!configFile)
        {
            ConfigManagerLogger->log('E', "Config file not found");
            return;
        }
        LogManager.log('S', "Config file found, parsing values");
        string line;
        while (getline(configFile, line))
        {
            size_t pos = line.find('=');

            if (pos == string::npos)
                continue;

            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            parseLine(key, value);
        }
    }
    const char *getLogFileFullPath()
    {
        char *fullPath = new char[strlen(LogFilePath) + strlen(LogFileName) + 1];
        if (!LogFilePath)
        {
            ConfigManagerLogger->log('W', "LogFilePath not set, using default value");
            LogFilePath = dLogFilePath;
        }
        if (!LogFileName)
        {
            ConfigManagerLogger->log('W', "LogFileName not set, using default value");
            LogFileName = dLogFileName;
        }
        if (LogFilePath && LogFileName)
        {
            strcpy(fullPath, LogFilePath);
            strcat(fullPath, LogFileName);
        }
        return fullPath;
    }
};