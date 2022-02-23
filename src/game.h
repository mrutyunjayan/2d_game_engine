// #define GAME_IMPLEMENTATION once before in the platform layer before including this file

#ifndef GAME_H
#define GAME_H

#include "utils.h"

typedef struct game_VideoBuffer {
  void* memory;
  i32 width;
  i32 height;
  i32 pitch;
} game_VideoBuffer;

typedef struct game_Input {
  f32 deltaTime;
} game_Input;

internal void
game_run(game_VideoBuffer* videoBuffer,
         game_Input* input);

internal void
game_update(game_Input* input);

internal void
game_render();

#endif // GAME_H
