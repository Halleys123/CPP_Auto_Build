#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>
#include <unordered_map>
#include <fstream>

#include "../include/common_functions.h"

using namespace std;

namespace fs = filesystem;

void show_error_message(const std::string &message)
{
    cerr << "Error: " << message << endl;
}

void run_make_command(const string &file_path)
{
    // Extract the file name from the file path
    string file_name = fs::path(file_path).filename().string();

    // Read the config file to get the build command for the specific file
    ifstream config_file("../config.cfg");
    string line;
    string build_command = "make"; // Default build command

    if (config_file.is_open())
    {
        while (getline(config_file, line))
        {
            size_t pos = line.find('=');
            if (pos != string::npos)
            {
                string files = line.substr(0, pos);
                string command = line.substr(pos + 1);

                // Split the files by comma and check if file_name is in the list
                size_t start = 0, end;
                while ((end = files.find(',', start)) != string::npos)
                {
                    if (fs::path(files.substr(start, end - start)).filename().string() == file_name)
                    {
                        build_command = command;
                        break;
                    }
                    start = end + 1;
                }
                if (fs::path(files.substr(start)).filename().string() == file_name)
                {
                    build_command = command;
                    break;
                }
            }
        }
        config_file.close();
    }

    cout << "Running build command: " << build_command << endl;

    int result = system(build_command.c_str());
    if (result != 0)
    {
        show_error_message("The build failed due to some error in your code please check.");
    }
}

void monitor_files(const string &path, int interval)
{
    auto file_mod_times = get_file_mod_times(path);

    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(interval));
        unordered_map<string, time_t> current_file_mod_times;
        try
        {
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
                        run_make_command(file_path);
                    }
                    else if (file_mod_times[file_path] != current_mod_time)
                    {
                        string message = "File changed: " + file_path;
                        cout << message << endl;
                        log_change(message);
                        file_mod_times[file_path] = current_mod_time;
                        run_make_command(file_path);
                    }
                }
            }
        }
        catch (const fs::filesystem_error &e)
        {
            show_error_message(e.what());
        }

        for (auto it = file_mod_times.begin(); it != file_mod_times.end();)
        {
            if (current_file_mod_times.find(it->first) == current_file_mod_times.end())
            {
                string message = "File deleted or moved: " + it->first;
                cout << message << endl;
                log_change(message);
                it = file_mod_times.erase(it);
                run_make_command(it->first);
            }
            else
            {
                ++it;
            }
        }
    }
}
