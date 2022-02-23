#ifndef SDL2_MAIN_H
#define SDL2_MAIN_H

//~ STRUCTS

typedef struct sdl2_Backbuffer {
  SDL_Texture* texture;
  void* memory;
  i32 width;
  i32 height;
  i32 pitch;
} sdl2_Backbuffer;

#endif // SDL2_MAIN_H

