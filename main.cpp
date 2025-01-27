#include <iostream>
#include <fstream>
#include "./src/ConfigManager.cpp"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config file>" << std::endl;
        return 1;
    }
    ifstream fileReader(argv[1]);
    if (!fileReader)
    {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }
    Logger Log(argv[1]);
    ConfigManager configmanager(Log, "./config.cfg");
    return 0;
}