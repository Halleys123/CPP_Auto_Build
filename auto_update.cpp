#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <fstream>

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
void run_make_command()
{
    system("make");
}
void monitor_files(const string &path)
{
    auto file_mod_times = get_file_mod_times(path);

    while (true)
    {
        this_thread::sleep_for(chrono::seconds(1));
        for (const auto &entry : fs::recursive_directory_iterator(path))
        {
            if (entry.path().extension() == ".cpp")
            {
                auto current_mod_time = fs::last_write_time(entry).time_since_epoch().count();
                auto file_path = entry.path().string();
                if (file_mod_times[file_path] != current_mod_time)
                {
                    cout << "File changed: " << file_path << endl;
                    file_mod_times[file_path] = current_mod_time;
                    run_make_command();
                }
            }
        }
    }
}

int main()
{
    string path = "./";
    monitor_files(path);
    return 0;
}
