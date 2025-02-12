SRC = ./src/*.cpp ./src/blocks/*.cpp ./src/core/*.cpp ./src/graphics/*.cpp ./src/systems/*.cpp ./src/utility/*.cpp ./src/world/*.cpp

all:
	g++ -g -std=c++20 ./third_party/glad.c -I ./third_party/include -I ./include -L ./lib $(SRC) -O3 -o ./bin/prog -lmingw32 -lSDL2main -lSDL2 -lSDL2_image	