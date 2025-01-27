# ? Directories to include and source files
include_dir = -I./include
src_dir = ./src

# ? Files to include
include_files = $(src_dir)/Logger.cpp $(src_dir)/ConfigManager.cpp $(src_dir)/BuildDaemon.cpp $(src_dir)/Colors.cpp

build: main.cpp
	g++ -o build.exe main.cpp ${include_files} $(include_dir)

# Generate dll file from test.cpp file and run it
test:
	g++ -shared -o test.dll test.cpp $(include_dir)