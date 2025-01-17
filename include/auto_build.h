#ifndef AUTO_BUILD_H
#define AUTO_BUILD_H

#include <string>

#include "../src/auto_build.cpp"

void run_make_command();
void monitor_files(const std::string &path, int interval);

#endif // AUTO_BUILD_H
