#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>

#include "../include/common_functions.h"

using namespace std;

namespace fs = filesystem;

void run_make_command()
{
    system("make");
}

void monitor_files(const string &path, int interval)
{
    auto file_mod_times = get_file_mod_times(path);

    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(interval));
        unordered_map<string, time_t> current_file_mod_times;
        for (const auto &entry : fs::recursive_directory_iterator(path))
        {
            if (entry.path().extension() == ".cpp")
            {
                auto current_mod_time = fs::last_write_time(entry).time_since_epoch().count();
                auto file_path = entry.path().string();
                current_file_mod_times[file_path] = current_mod_time;

                if (file_mod_times.find(file_path) == file_mod_times.end())
                {
                    string message = "File added: " + file_path;
                    cout << message << endl;
                    log_change(message);
                    file_mod_times[file_path] = current_mod_time;
                    run_make_command();
                }
                else if (file_mod_times[file_path] != current_mod_time)
                {
                    string message = "File changed: " + file_path;
                    cout << message << endl;
                    log_change(message);
                    file_mod_times[file_path] = current_mod_time;
                    run_make_command();
                }
            }
        }

        for (auto it = file_mod_times.begin(); it != file_mod_times.end();)
        {
            if (current_file_mod_times.find(it->first) == current_file_mod_times.end())
            {
                string message = "File deleted or moved: " + it->first;
                cout << message << endl;
                log_change(message);
                it = file_mod_times.erase(it);
                run_make_command();
            }
            else
            {
                ++it;
            }
        }
    }
}
