#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

typedef struct Window
{
    SDL_Window *window;
    SDL_GLContext gl_context;
} Window;

Window *Window_Alloc();
bool Window_Init(Window *renderer, const char *title, int width, int height);

void Window_Clear(float r, float g, float b, float a);
void Window_Swap(Window *renderer);

void Window_Deinit(Window *renderer);
void Window_Destroy(Window *renderer);