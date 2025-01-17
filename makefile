include_dir = -I./include

BUILD = WIN
# BUILD = GEN

build: main.cpp
	g++ -o build main.cpp $(include_dir) -D$(BUILD)