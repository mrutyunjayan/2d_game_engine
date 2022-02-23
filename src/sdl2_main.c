#include <stdio.h>
#include <malloc.h>
#include <x86intrin.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#define NK_IMPLEMENTATION
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-pointer-subtraction"
#include <nuklear.h>
#pragma clang diagnostic pop

#define JAI_UTILS_IMPLEMENTATION
#include "utils.h"
#include "sdl2_main.h"

#include "game.c"

#define BYTES_PER_PIXEL 4
#define TARGET_HZ 60


//~ GLOBALS
global b32 running = true;
global u64 perfCounterFrequency;
global readOnly f32 targetMillisecondsPerFrame = (1000.0f / (f32)TARGET_HZ);

internal void
sdl2_handle_events() {
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

internal i32
sdl2_get_windowRefreshRate(SDL_Window* window)
{
    SDL_DisplayMode mode;
    i32 displayIndex = SDL_GetWindowDisplayIndex(window);
    int defaultRefreshRate = 60;
    if (SDL_GetDesktopDisplayMode(displayIndex, &mode) != 0) {
        return defaultRefreshRate;
    }
    if (mode.refresh_rate == 0) {
        return defaultRefreshRate;
    }
    
    return mode.refresh_rate;
}

internal b32
sdl2_resize_texture(SDL_Renderer* renderer,
                    sdl2_Backbuffer* backbuffer) {
    if (backbuffer->texture) SDL_DestroyTexture(backbuffer->texture);
    backbuffer->texture = SDL_CreateTexture(renderer,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            backbuffer->width, backbuffer->height);
    if (!backbuffer->texture) {
        printf("error init texture\n");
        return false;
    }
    
    if (backbuffer->memory) free(backbuffer->memory);
    backbuffer->memory = malloc(backbuffer->width * backbuffer->height * BYTES_PER_PIXEL);
    if (!backbuffer->memory) {
        printf("error allocating backbuffer");
        return false;
    }
    
    return true;
}

internal b32
sdl2_init_platform(SDL_Window** window,
                   SDL_Renderer** renderer,
                   sdl2_Backbuffer* backbuffer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error init sdl\n");
        return false;
    }
    
    *window = SDL_CreateWindow("2D-Engine",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               backbuffer->width, backbuffer->height,
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
    
    if(!sdl2_resize_texture(*renderer, backbuffer)) return false;
    
    return true;
}

internal b32
sdl2_render(SDL_Renderer* renderer,
            sdl2_Backbuffer backbuffer) {
    i32 error = SDL_UpdateTexture(backbuffer.texture, 0, backbuffer.memory, backbuffer.pitch);
    if (error) {
        printf("error %d while updating sdl texture\n", error);
        return  false;
    }
    
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backbuffer.texture, 0, 0);
    SDL_RenderPresent(renderer);
    
    return true;
}

internal f32
sdl2_get_millisecondsElapsed(u64 lastCount) {
    f32 result = 1000.0f *
    (f32)(SDL_GetPerformanceCounter() - lastCount) / (f32)perfCounterFrequency;
    return result;
}

internal void
sdl2_enforce_frameRate(u64 lastCount) {
    f32 msElapsed = sdl2_get_millisecondsElapsed(lastCount);
    while ((msElapsed < targetMillisecondsPerFrame)) {
        u32 timeToSleep = (u32)(targetMillisecondsPerFrame - msElapsed);
        SDL_Delay(timeToSleep);
        msElapsed = sdl2_get_millisecondsElapsed(lastCount);
    }
}

int
main(int argCount, char** args) {
    perfCounterFrequency = SDL_GetPerformanceFrequency();
    SDL_Window* window = 0;
    SDL_Renderer* renderer = 0;
    i32 windowWidth =  640;
    i32 windowHeight = 480;
    sdl2_Backbuffer backbuffer = {
        .width = windowWidth,
        .height = windowHeight,
        .pitch = windowWidth * BYTES_PER_PIXEL
    };
    if (!sdl2_init_platform(&window, &renderer, &backbuffer)) return -1;
    
    game_VideoBuffer gameVideoBuffer = {
        .width = backbuffer.width,
        .height = backbuffer.height,
        .pitch = backbuffer.pitch,
        .memory = backbuffer.memory
    };
    
    u64 perfCountStart = SDL_GetPerformanceCounter();
    u64 perfCountEnd = SDL_GetPerformanceCounter();
    u64 perfCount = 0;
    
    game_Input input = {0};
    
    while (running) {
        sdl2_handle_events();
        game_run(&gameVideoBuffer, &input);
        sdl2_render(renderer, backbuffer);
        // TODO (Jai): Investigate why the location if the sdl2_enforce_frameRate function
        // affects it's use
        sdl2_enforce_frameRate(perfCountStart);
        
        perfCountEnd = SDL_GetPerformanceCounter();
        perfCount = perfCountEnd - perfCountStart;
        f32 fps = (f32)perfCounterFrequency / (f32)perfCount;
        f32 msPerFrame = 1000.0f * ((f32)perfCount / (f32)perfCounterFrequency);
        printf("fps: %f ms/f: %f\n\n", fps, msPerFrame);
        perfCountStart = perfCountEnd;
    }
    
    SDL_Quit();
    return 0;
}
