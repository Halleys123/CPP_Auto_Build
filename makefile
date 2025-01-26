include_dir = -I./include

BUILD = WIN
# BUILD = GEN

build: main.cpp
	g++ -o build.exe main.cpp $(include_dir) -D$(BUILD)

# Generate dll file from test.cpp file and run it
test:
	g++ -shared -o test.dll test.cpp $(include_dir)