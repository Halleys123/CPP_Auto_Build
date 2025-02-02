# ? Directories to include and source files
include_dir = -I./include
src_dir = ./src
build_dir = ./bin

# ? Files to include
include_files =  $(src_dir)/ConfigManager.cpp $(src_dir)/BuildDaemon.cpp

complete_build: main.cpp
	g++ -o ${build_dir}/build.exe main.cpp -L${build_dir} -lLogger $(include_files) $(include_dir)

# Generate dll file from test.cpp file and run it
# test:
# 	g++ -shared -o test.dll test.cpp $(include_dir)

Logger: $(src_dir)/Logger.cpp
	g++ -shared -o ${build_dir}/Logger.dll $(src_dir)/Logger.cpp $(include_dir)

test:
	echo "Working test"