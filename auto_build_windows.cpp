#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

namespace fs = filesystem;
void show_error_message(const std::string &message)
{
    HWND hwnd = CreateWindowEx(
        0,
        "STATIC",
        NULL,
        WS_OVERLAPPEDWINDOW,
        0, 0, 0, 0,
        NULL,
        NULL,
        NULL,
        NULL);

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);

    MessageBox(
        hwnd,
        message.c_str(),
        "Error Notification",
        MB_ICONERROR | MB_OK);

    DestroyWindow(hwnd);
}
unordered_map<string, time_t> get_file_mod_times(const string &path)
{
    unordered_map<string, time_t> file_mod_times;
    try
    {
        for (const auto &entry : fs::recursive_directory_iterator(path))
        {
            if (entry.path().extension() == ".cpp" && entry.path().filename() != "auto_update.cpp")
            {
                file_mod_times[entry.path().string()] = fs::last_write_time(entry).time_since_epoch().count();
            }
        }
    }
    catch (const fs::filesystem_error &e)
    {
        show_error_message(e.what());
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
    ofstream log_file("auto_update_history.log", ios_base::app);
    log_file << "[" << get_current_time() << "] " << message << endl;
}

void run_make_command()
{
    int result = system("make");
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
                run_make_command();
            }
            else
            {
                ++it;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    string path = "./";
    int interval;

    if (argc < 2)
    {
        interval = 1;
    }
    else
        interval = stoi(argv[1]) ? stoi(argv[1]) : 100;

    printf("Using %d miliseconds as interval for update", interval);
    log_change("Monitoring started with interval: " + to_string(interval) + " milliseconds.");
    monitor_files(path, interval);
    return 0;
}
