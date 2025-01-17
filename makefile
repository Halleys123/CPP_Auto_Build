include_dir = -I./include

build: main.cpp
	g++ -o build main.cpp $(include_dir) -D$(BUILD)