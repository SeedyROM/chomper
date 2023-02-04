#include "window.h"

#include "common.h"

Window *Window_Alloc()
{
    TRY_ALLOC(renderer, Window, 1);
    return renderer;
}

bool Window_Init(Window *window, const char *title, int width, int height)
{
    // Initialize SDL
    TRY_SDL(SDL_Init(SDL_INIT_VIDEO) == 0);

    // Setup OpenGL attributes
    TRY_SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) == 0);
    TRY_SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2) == 0);
    TRY_SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) == 0);
    TRY_SDL(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == 0);

    // Create the SDL window.
    window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    TRY_SDL(window->window != NULL);

    // Create the OpenGL context.
    window->gl_context = SDL_GL_CreateContext(window->window);
    TRY_SDL(window->gl_context != NULL);

    // Setup OpenGL viewport.
    glViewport(0, 0, width, height);

    // Setup double buffering.
    TRY_SDL(SDL_GL_SetSwapInterval(1) == 0);

    return true;
}

void Window_Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window_Swap(Window *window)
{
    SDL_GL_SwapWindow(window->window);
}

void Window_Deinit(Window *window)
{
    SDL_GL_DeleteContext(window->gl_context);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}

void Window_Destroy(Window *window)
{
    free(window);
}