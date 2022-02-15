INCLUDE = -I./include/ 
CC = clang
WARNINGS = -Wall -Wextra -Wno-unused-parameter -Wno-undef -Wno-unused-macros -Wno-extra-semi-stmt -Wno-unused-function
CFLAGS = `sdl2-config --cflags --libs` $(INCLUDE) $(WARNINGS) src/sdl2_main.c -g
LFLAGS = -lm

build: src/*.c
	$(CC) $(CFLAGS) $(LFLAGS) -o build/2D-Engine 

run:
	./build/2D-Engine

clean:
	rm ./build/2D-Engine

.PHONY: build run clean
