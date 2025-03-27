CC = g++
CFLAGS = -std=c++20 -O3 -g
TARGETS = ray_casting perlin
LIBRARIES = -I ../third_party/include -I ../include -L ../lib
LDLIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
SOURCES := $(wildcard ../src/utility/*.cpp)
GLAD = ../third_party/glad.c

all: $(TARGETS)

%: %.cpp
	$(CC) $(CFLAGS) $(GLAD) $(LIBRARIES) $(SOURCES) -o $@ $< $(LDLIBS)
