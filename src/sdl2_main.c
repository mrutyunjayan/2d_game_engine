#include <SDL2/SDL.h>

#define STB_TRUETYPE_IMPLEMENTATION  
#include <stb_truetype.h>
#define NK_IMPLEMENTATION
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-pointer-subtraction"
#include <nuklear.h>
#pragma clang diagnostic pop

#define JAI_UTILS_IMPLEMENTATION
#include "utils.h"
#define GAME_IMPLEMENTATION
#include "game.h"

global b32 running = true;

#include <stdio.h>
#include <malloc.h>

internal void
sdl2_processInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        running = false;
        } break;
      case SDL_KEYDOWN:
      case SDL_KEYUP: {
        SDL_Keycode keycode = event.key.keysym.sym;
        switch (keycode) {
          case SDLK_ESCAPE: {
            running = false;
          } break;
        }
        } break;
    }
  }
}

internal b32
sdl2_init_platform(SDL_Window** window,
                   SDL_Renderer** renderer,
                   i32 windowWidth, i32 windowHeight) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error init sdl\n");
        return false;
      }

      *window = SDL_CreateWindow("2D-Engine",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                windowWidth, windowHeight,
                                SDL_WINDOW_BORDERLESS);
      if (!*window) {
        printf("error init window\n");
        return false;
      }
      
      *renderer = SDL_CreateRenderer(*window, -1, 0);
      if (!*renderer) {
        printf("error init renderer\n");
        return false;
      }

      return true;
}

internal b32
sdl2_init_backBuffer(SDL_Texture** texture,
                    SDL_Renderer* renderer,
                    i32 width, i32 height,
                    void** pixels) {
  *texture = SDL_CreateTexture(renderer, 
                              SDL_PIXELFORMAT_ABGR8888,
                              SDL_TEXTUREACCESS_STREAMING,
                              width, height);
  if (!*texture) {
    printf("error init texture\n");
    return false;
  }
  *pixels = malloc(width * height * 4);
  if (!*pixels) {
    printf("error allocating backbuffer\n");
    return false;
  }
  return true;
}

int
main(int argCount, char** args) { 
    SDL_Window* window = 0;
    SDL_Renderer* renderer = 0;
    i32 windowWidth =  640;
    i32 windowHeight = 480;
    void* pixels = 0;
    if(!sdl2_init_platform(&window, &renderer, windowWidth, windowHeight)) return -1; 
    SDL_Texture* texture = 0;
    if(!sdl2_init_backBuffer(&texture, renderer, windowWidth, windowHeight, &pixels)) return -1;
    while (running) {
    sdl2_processInput();
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    // game_updateAndRender();
  }

  SDL_Quit();
  return 0;
}
