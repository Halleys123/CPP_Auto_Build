#ifndef AUTO_BUILD_WINDOWS_H
#define AUTO_BUILD_WINDOWS_H

#include <string>

#include "../include/common_functions.h"
#include "../src/auto_build_windows.cpp"

void show_error_message(const std::string &message);
void run_make_command();
void monitor_files(const std::string &path, int interval);

#endif // AUTO_BUILD_WINDOWS_H
