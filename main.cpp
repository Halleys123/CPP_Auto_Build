#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "./include/common_functions.h"

#ifndef _WIN32
#include "./include/auto_build.h"
#elif defined(WIN)
#include "./include/auto_build_windows.h"
#else
#include "./include/auto_build.h"
#endif

using namespace std;

void monitor_files(const string &path, int interval);

void read_config_value(const string &key, string &value);
void set_config_value(const string &key, const string &value);
void initialize_config(string &interval_str, string &user_interval_str, string &enable_logging_str, string &log_file_str);
void process_arguments(int argc, char *argv[], int &interval, bool &enable_logging);
bool is_config_valid();
void use_default_config(string &interval_str, string &user_interval_str, string &enable_logging_str);

int main(int argc, char *argv[])
{
    // TODO(HALLEYS): Add path option to config.cfg so that user can change what directories to monitor.
    string path = "./";
    string interval_str = "100";                     // Default interval as string
    string user_interval_str = "-1";                 // Default user interval as string
    string enable_logging_str = "false";             // Default logging disabled as string
    string log_file_str = "auto_update_history.log"; // Default log file name

    if (!is_config_valid())
    {
        use_default_config(interval_str, user_interval_str, enable_logging_str);
    }
    else
    {
        initialize_config(interval_str, user_interval_str, enable_logging_str, log_file_str);
    }

    int interval = stoi(interval_str);
    int user_interval = stoi(user_interval_str);
    bool enable_logging = (enable_logging_str == "true");

    if (user_interval != -1)
    {
        interval = user_interval;
    }

    process_arguments(argc, argv, interval, enable_logging);

    read_config_value("USER_TIME", user_interval_str);
    user_interval = stoi(user_interval_str);
    if (user_interval > 0)
    {
        interval = user_interval;
    }
    else
    {
        cout << "User Interval in configuration file is invalid using default." << endl;
    }

    printf("Using %d milliseconds as interval for update\n", interval);
    if (enable_logging)
    {
        log_change("Monitoring started with interval: " + to_string(interval) + " milliseconds.", log_file_str);
    }
    monitor_files(path, interval);
    return 0;
}

bool is_config_valid()
{
    ifstream config_file("config.cfg");
    string line;
    if (config_file.is_open())
    {
        while (getline(config_file, line))
        {
            // Skip comments
            if (line.find('#') == 0)
            {
                continue;
            }

            size_t equal_pos = line.find('=');
            if (equal_pos == string::npos)
            {
                cout << "Configuration file is invalid using defaults." << endl;
                return false;
            }
            // Check if there is a colon after the equal sign
            size_t colon_pos = line.find(':', equal_pos);
            if (colon_pos != string::npos && colon_pos < line.length() - 1)
            {
                continue; // Valid entry with colon after equal sign
            }
        }
        config_file.close();
    }
    return true;
}

void use_default_config(string &interval_str, string &user_interval_str, string &enable_logging_str)
{
    interval_str = "100";
    user_interval_str = "-1";
    enable_logging_str = "false";
}

void initialize_config(string &interval_str, string &user_interval_str, string &enable_logging_str, string &log_file_str)
{
    read_config_value("DEFAULT_TIME", interval_str);
    read_config_value("USER_TIME", user_interval_str);
    read_config_value("GENERATE_LOG", enable_logging_str);
    read_config_value("LOG_FILE", log_file_str);
}

void process_arguments(int argc, char *argv[], int &interval, bool &enable_logging)
{
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if ((arg == "--interval" || arg == "-i") && i + 1 < argc)
        {
            interval = stoi(argv[++i]);
            set_config_value("USER_TIME", to_string(interval));
            if (interval < 0)
            {
                string interval_str;
                read_config_value("DEFAULT_TIME", interval_str);
                interval = stoi(interval_str);
                if (interval < 0)
                {
                    cout << "Invalid config file detected, using 100 ms." << endl;
                    interval = 100;
                }
            }
        }
        else if (arg == "--log" || arg == "-l")
        {
            if (i + 1 < argc)
            {
                string logging = argv[++i];
                if (logging == "false")
                    enable_logging = false;
                else
                    enable_logging = true;
            }
            else
                enable_logging = true;
            set_config_value("GENERATE_LOG", enable_logging ? "true" : "false");
        }
        else if (arg == "--default_interval" || arg == "-df" && i + 1 < argc)
        {
            string default_time_str = argv[++i];
            set_config_value("DEFAULT_TIME", default_time_str);
            interval = stoi(default_time_str);
        }
        else if (arg == "--help" || arg == "-h")
        {
            cout << "Usage: " << argv[0] << " [options]\n";
            cout << "Options:\n";
            cout << "  -i, --interval           <milliseconds>       Set the monitoring interval\n";
            cout << "  -l, --log                <boolean|optional>   Enable logging if value is not provided\n";
            cout << "  -df,--default_interval   <number>             Set default interval used by program if user interval is invalid.\n";
            cout << "  -i,--interval            <number>             Set interval that will be used by program.\n";
            cout << "  -h, --help               <noargs>             Show this help message\n";
            exit(0);
        }
    }
}

void read_config_value(const string &key, string &value)
{
    ifstream config_file("config.cfg");
    string line;
    if (config_file.is_open())
    {
        while (getline(config_file, line))
        {
            if (line.find(key) != string::npos)
            {
                istringstream iss(line);
                string config_key, config_value;
                if (getline(iss, config_key, '=') && getline(iss, config_value))
                {
                    size_t pos = config_value.find(':');
                    if (pos != string::npos)
                    {
                        value = config_value.substr(0, pos);
                    }
                    else
                    {
                        value = config_value;
                    }
                    break;
                }
            }
        }
    }
}

void set_config_value(const string &key, const string &value)
{
    ifstream config_file("config.cfg");
    string line;
    string content;
    bool found = false;

    if (config_file.is_open())
    {
        while (getline(config_file, line))
        {
            if (line.find(key) != string::npos)
            {
                size_t pos = line.find('=');
                size_t colon_pos = line.find(':');
                if (colon_pos != string::npos && colon_pos > pos)
                {
                    content += key + "=" + value + line.substr(colon_pos) + "\n";
                }
                else
                {
                    content += key + "=" + value + "\n";
                }
                found = true;
            }
            else
            {
                content += line + "\n";
            }
        }
        config_file.close();
    }

    if (!found)
    {
        content += key + "=" + value + "\n";
    }

    ofstream config_file_out("config.cfg");
    if (config_file_out.is_open())
    {
        config_file_out << content;
        config_file_out.close();
    }
}
