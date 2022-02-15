// #define GAME_IMPLEMENTATION once before in the platform layer before including this file

#ifndef GAME_H
#define GAME_H

#include "utils.h"

typedef struct Game {

} Game; 

typedef struct game_Input {

} game_Input;

internal Game
game_initialize();

internal void
game_updateAndRender(game_Input* input);

internal void
game_update(game_Input* input);

internal void
game_render();

#ifdef GAME_IMPLEMENTATION



#undef GAME_IMPLEMENTATION
#endif // GAME_IMPLEMENTATION

#endif // GAME_H
