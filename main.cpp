#include <iostream>
#include <string>

#include "./include/common_functions.h"

#if defined(WIN)
#include "./include/auto_build_windows.h"
#elif defined(GEN)
#include "./include/auto_build.h"
#endif

using namespace std;

void monitor_files(const string &path, int interval);
void log_change(const std::string &message);

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

    printf("Using %d milliseconds as interval for update\n", interval);
    log_change("Monitoring started with interval: " + to_string(interval) + " milliseconds.");
    monitor_files(path, interval);
    return 0;
}
