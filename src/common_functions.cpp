#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

namespace fs = filesystem;

unordered_map<string, time_t> get_file_mod_times(const string &path)
{
    unordered_map<string, time_t> file_mod_times;
    for (const auto &entry : fs::recursive_directory_iterator(path))
    {
        if (entry.path().extension() == ".cpp" && entry.path().filename() != "auto_update.cpp")
        {
            file_mod_times[entry.path().string()] = fs::last_write_time(entry).time_since_epoch().count();
        }
    }
    return file_mod_times;
}

string get_current_time()
{
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now_time));
    return string(buf);
}

void log_change(const string &message)
{
    ifstream config_file("config.cfg");
    string line;
    bool generate_log = false;

    if (config_file.is_open())
        while (getline(config_file, line))
        {
            if (line.find("GENERATE_LOG") != string::npos)
            {
                istringstream iss(line);
                string key, value;
                if (getline(iss, key, '=') && getline(iss, value))
                {
                    generate_log = (value == "true");
                    printf("Logging is: %s\n", generate_log ? "Enabled" : "Disabled");
                }
            }
        }

    if (generate_log)
    {
        ofstream log_file("auto_update_history.log", ios_base::app);
        log_file << "[" << get_current_time() << "] " << message << endl;
    }
}
