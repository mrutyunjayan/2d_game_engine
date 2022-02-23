#include "game.h"

internal void
test_fill_red(game_VideoBuffer* backbuffer) {
  u32 red = 0xFFFF2211;
  u8* row = backbuffer->memory;
  for (i32 i = 0; i < backbuffer->height; i++) {
    u32* pixel = (u32*)row;
    for (i32 j = 0; j < backbuffer->width; j++) {
      *pixel = red;
      pixel++;
    }
    row += backbuffer->pitch;
  }
}


internal void
game_run(game_VideoBuffer* videoBuffer,
         game_Input* input) {
  test_fill_red(videoBuffer);
}

internal void
game_update(game_Input* input){}

internal void
game_render(){}

