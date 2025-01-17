# Auto Update

## Overview

- `auto_build` is a C++ utility that monitors `.cpp` files in a specified directory for changes. When a change is detected, it automatically runs the `make` command to rebuild the project. This tool is useful for developers who want to automate the build process during development, ensuring that their project is always up-to-date with the latest changes.
- There are two options when using the tool you can either use the General version that works on all machines including, UNIX, Linux, Mac, and Windows or you can use the Windows specific version that works only on Windows machines. That are running at least `Windows 2000 Professional` or in technical terms at least support `Win32 API`. Although for now there are not many differences in this version **for now** but provides you with a Windows style MessageBox that alerts you when your build fails.

## How It Works

1. **File Scanning**: The program scans the specified directory and its subdirectories for `.cpp` files.
2. **Record Modification Times**: It records the last modification times of these files.
3. **Monitoring Loop**: It enters a loop where it periodically checks the modification times of the files.
4. **Detect Changes**: If a change is detected in any of the `.cpp` files, it prints the file path to the console and runs the `make` command to rebuild the project.

## How to Use

1. **Clone the Repository**: Clone the repository containing the `auto_build` utility to your local machine.
   ```sh
   git clone https://github.com/Halleys123/CPP_Auto_Build
   cd auto_build
   ```
2. **Compile the Program**: Compile `main.cpp` using using g++ compiler, by using the given make file:
   ```sh
      make build
   ```
   - You can use above command if you want to build general version of the program.
   - As for Windows users you can use to build the Windows specific version of the program:
   ```sh
   make build BUILD=WIN
   ```
3. **Run the Program**: Execute the compiled binary, specifying the directory to monitor. By default, it monitors the current directory (`./`):
   ```sh
   ./build.exe
   ```
4. **Modify Files**: Make changes to any `.cpp` files in the monitored directory. The program will detect the changes and automatically run the `make` command.

## Setting Flags

You can set various flags when running the program:

- **Interval**: Set the monitoring interval in milliseconds.
  ```sh
  ./auto_build --interval 200
  ```
- **Enable Logging**: Enable or disable logging.
  ```sh
  ./auto_build --log true
  ```
- **Log File**: Set the log file path.
  ```sh
  ./auto_build --log_file log.txt
  ```
- **Default Interval**: Set the default interval used by the program if the user interval is invalid.
  ```sh
  ./auto_build --default_interval 100
  ```
- **Help**: Display the help message.
  ```sh
  ./auto_build --help
  ```

## Requirements

- **C++17 or later**: Ensure your compiler supports C++17 or later.
- **Make Utility**: A `make` utility must be installed on your system to build the project.

## Notes

- **Make Configuration**: Ensure that the `make` command is properly configured to build your project. This typically involves having a `Makefile` in the root of your project directory.
- **Monitoring Interval**: The monitoring interval is set to 100 miliseconds. You can adjust this interval by using the `-df <interval>` flag, where `<interval>` is the desired interval in miliseconds.
- **Logging**: By default, logging is disabled. You can enable logging by using the `-log true` flag.

## Example

## Example

Here is an example of how to use `auto_build`:

1. **Create your Project Folder**: In the given command block, a file called `main.cpp` is generated.
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
