#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <string>
#include <unordered_map>

#include "../src/common_functions.cpp"

std::unordered_map<std::string, time_t> get_file_mod_times(const std::string &path);
std::string get_current_time();
void log_change(const std::string &message);

#endif // COMMON_FUNCTIONS_H
