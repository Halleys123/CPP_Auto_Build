#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>
#include <unordered_map>
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used APIs

#ifdef _WIN32
#include <windows.h>
#endif

#include "../include/common_functions.h"

using namespace std;

namespace fs = filesystem;

void show_error_message(const std::string &message)
{
    HWND hwnd = CreateWindowEx(
        0,
        TEXT("STATIC"),
        NULL,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 100, 100,
        NULL,
        NULL,
        NULL,
        NULL);

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    SetForegroundWindow(hwnd);

    MessageBoxW(
        hwnd,
        wstring(message.begin(), message.end()).c_str(),
        L"Error Notification",
        MB_ICONERROR | MB_OK);

    DestroyWindow(hwnd);
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
