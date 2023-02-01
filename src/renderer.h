#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

typedef struct Renderer
{
    SDL_Window *window;
    SDL_GLContext gl_context;
} Renderer;

Renderer *Renderer_Alloc();
bool Renderer_Init(Renderer *renderer, const char *title, int width, int height);
void Renderer_Destroy(Renderer *renderer);
void Renderer_Dealloc(Renderer *renderer);