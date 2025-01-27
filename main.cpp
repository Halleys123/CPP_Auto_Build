#include <iostream>
#include <fstream>
#include "./src/Logger.cpp"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config file>" << std::endl;
        return 1;
    }
    // Checking if file exists
    ifstream fileReader(argv[1]);
    if (!fileReader)
    {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }
    Logger log(argv[1]);
    while (true)
        ;
    return 0;
}