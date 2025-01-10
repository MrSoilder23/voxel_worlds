all:
	g++ -g -std=c++20 ./third_party/glad.c -I ./third_party/include -I ./include -L ./lib ./src/*.cpp -O3 -o ./bin/prog -lmingw32 -lSDL2main -lSDL2 -lSDL2_image	