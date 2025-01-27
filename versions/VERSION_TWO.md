# Building Deatils

## General instruction

1. Don't use file name directly anywhere, always use the file path that will be relative to the executable directory.

## ConfigManager

- Program will have a ConfigManager class that will have the responsiblity to read and parse the configuration file. Here parsing refers to reading an appropriate data structure maybe like a map, table or a struct.
- It will also have the responsibility to validate the configuration file.
- It will contain functions to get the values of the configuration file.
- Key data present in the config.cfg are:
  1. INTERVAL
  2. LOGFILEPATH - This will be the path where the log file will be created (Name of file should not be present in the path).
  3. LOGFILENAME - This will be the name of the log file.
  4. ROOTDIRECTORY - This will be the root directory of the project. If not provided it will be the directory where the executable is present.
  5. SpecialCommands - Format is `file_name.cpp=command` or `file_name1.cpp/file_name2.cpp=command`
  6. EXCLUDEDIRECTORIES - This will be the directories that will be excluded from monitoring, Path should not contain file name.
  7. EXCLUDEFILES - This will be the files that will be excluded from monitoring. This will also be a path to files but must contain file name.
- Methods in ConfigManager:
  1. `bool readConfigFile()`: This will read the configuration file and parse it.
  2. `bool validateConfigFile()`: This will validate the configuration file.
  3. `string getInterval()`: This will return the interval value.
  4. `string getLogFilePath()`: This will return the log file path.
  5. `string getLogFileName()`: This will return the log file name.
  6. `string getRootDirectory()`: This will return the root directory.
  7. `map<string, string> getSpecialCommands()`: This will return the special commands.
  8. `vector<string> getExcludeDirectories()`: This will return the excluded directories.
  9. `vector<string> getExcludeFiles()`: This will return the excluded files.

## FileMonitor

- Primary job of this class will be to read the files and directories and check if any changes have been made.
- It will have a function that will recursively read all the files and directories and store the last modified time of the files.

## ActionExecutor

- This class will have the responsibility to execute the actions that are to be taken when a change is detected.

## Logger

- This class will have the responsibility to log the messages to the log file.
- It will have functions to log messages of different types like info, error, warning etc.

## BuildDaemon

- This class will be the main class that will have the responsibility to run the program.
