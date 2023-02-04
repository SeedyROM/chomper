#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

typedef struct TextureInfo
{
    int width;
    int height;
    int channels;
} TextureInfo;

typedef struct Texture
{
    GLuint id;
    TextureInfo info;
    bool loaded;
} Texture;

Texture *Texture_Alloc();
bool Texture_Load(Texture *texture, const char *path);
bool Texture_Unload(Texture *texture);
void Texture_Destroy(Texture *texture);
