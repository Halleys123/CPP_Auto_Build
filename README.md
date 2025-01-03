# Auto Update

## Overview

`auto_update` is a C++ utility that monitors `.cpp` files in a specified directory for changes. When a change is detected, it automatically runs the `make` command to rebuild the project. This tool is useful for developers who want to automate the build process during development, ensuring that their project is always up-to-date with the latest changes.

## How It Works

1. **File Scanning**: The program scans the specified directory and its subdirectories for `.cpp` files, excluding `auto_update.cpp` itself.
2. **Record Modification Times**: It records the last modification times of these files.
3. **Monitoring Loop**: It enters a loop where it periodically checks the modification times of the files.
4. **Detect Changes**: If a change is detected in any of the `.cpp` files, it prints the file path to the console and runs the `make` command to rebuild the project.

## How to Use

1. **Clone the Repository**: Clone the repository containing the `auto_update` utility to your local machine.
   ```sh
   git clone https://github.com/Halleys123/CPP_Auto_Build
   cd auto_update
   ```
2. **Compile the Program**: Compile `auto_update.cpp` using your preferred C++ compiler. For example:
   ```sh
   g++ -std=c++17 -o auto_update auto_update.cpp
   ```
3. **Run the Program**: Execute the compiled binary, specifying the directory to monitor. By default, it monitors the current directory (`./`):
   ```sh
   ./auto_update
   ```
4. **Modify Files**: Make changes to any `.cpp` files in the monitored directory. The program will detect the changes and automatically run the `make` command.

## Requirements

- **C++17 or later**: Ensure your compiler supports C++17 or later.
- **Make Utility**: A `make` utility must be installed on your system to build the project.

## Notes

- **Make Configuration**: Ensure that the `make` command is properly configured to build your project. This typically involves having a `Makefile` in the root of your project directory.
- **Monitoring Interval**: The monitoring interval is set to 1 second. You can adjust this interval by modifying the `sleep_for` duration in the `monitor_files` function.

## Example

Here is an example of how to use `auto_update`:

1. **Create a Sample Project**:
   ```sh
   mkdir sample_project
   cd sample_project
   echo -e '#include <iostream>\nint main() { std::cout << "Hello, World!"; return 0; }' > main.cpp
   echo -e 'all:\n\tg++ -o main main.cpp' > Makefile
   ```
2. **Compile and Run `auto_update`**:
   ```sh
   g++ -std=c++17 -o auto_update ../auto_update.cpp
   ./auto_update
   ```
3. **Modify `main.cpp`**:
   ```sh
   echo -e '#include <iostream>\nint main() { std::cout << "Hello, Auto Update!"; return 0; }' > main.cpp
   ```
   The `auto_update` program will detect the change and automatically run the `make` command to rebuild the project.

## License

This project is licensed under the MIT License.
