# Auto Update

## Overview

- `auto_build` is a C++ utility that monitors `.cpp` files in a specified directory for changes. When a change is detected, it automatically runs the `make` command to rebuild the project. This tool is useful for developers who want to automate the build process during development, ensuring that their project is always up-to-date with the latest changes.
- `auto_build_windows.cpp` works similar to `auto_build.cpp` but it alerts you when your build fail by using window's MessageBox from `windows.h`, and as the name suggests it can be used on windows machine that are running at least `Windows 2000 Professional`.   
## How It Works

1. **File Scanning**: The program scans the specified directory and its subdirectories for `.cpp` files, excluding `auto_build.cpp` itself.
2. **Record Modification Times**: It records the last modification times of these files.
3. **Monitoring Loop**: It enters a loop where it periodically checks the modification times of the files.
4. **Detect Changes**: If a change is detected in any of the `.cpp` files, it prints the file path to the console and runs the `make` command to rebuild the project.

## How to Use

1. **Clone the Repository**: Clone the repository containing the `auto_build` utility to your local machine.
   ```sh
   git clone https://github.com/Halleys123/CPP_Auto_Build
   cd auto_build
   ```
2. **Compile the Program**: Compile `auto_build.cpp` using your preferred C++ compiler. For example:
   ```sh
   g++ -std=c++17 -o auto_build auto_build.cpp
   ```
3. **Run the Program**: Execute the compiled binary, specifying the directory to monitor. By default, it monitors the current directory (`./`):
   ```sh
   ./auto_build
   ```
4. **Modify Files**: Make changes to any `.cpp` files in the monitored directory. The program will detect the changes and automatically run the `make` command.

## Requirements

- **C++17 or later**: Ensure your compiler supports C++17 or later.
- **Make Utility**: A `make` utility must be installed on your system to build the project.

## Notes

- **Make Configuration**: Ensure that the `make` command is properly configured to build your project. This typically involves having a `Makefile` in the root of your project directory.
- **Monitoring Interval**: The monitoring interval is set to 1 second. You can adjust this interval by modifying the `sleep_for` duration in the `monitor_files` function.

## Example

Here is an example of how to use `auto_build`:

1. **Create your Project Folder**: In the given command block a file called main.cpp is generated.
   ```sh
   mkdir sample_project
   cd sample_project
   echo -e '#include <iostream>\nint main() { std::cout << "Hello, World!"; return 0; }' > main.cpp
   echo -e 'all:\n\tg++ -o main main.cpp' > Makefile
   ```
2. **Compile and Run `auto_build`**: Now you build the `auto_build.cpp` or `auto_build_windows.cpp` depending on your operating system.
   ```sh
   g++ -std=c++17 -o auto_build ../auto_build.cpp
   ./auto_build
   ```
3. **Modify `main.cpp`**:
   ```sh
   echo -e '#include <iostream>\nint main() { std::cout << "Hello, Auto Update!"; return 0; }' > main.cpp
   ```
   The `auto_build` program will detect the change and automatically run the `make` command to rebuild the project.

## License

This project is licensed under the MIT License.
