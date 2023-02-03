#pragma once

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <cglm/cglm.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>

#include "texture.h"
#include "shader.h"

typedef struct SpriteRenderer
{
    GLuint vao;
    GLuint vbo;
    Shader *shader;
    mat4 projection;
    mat4 view;
} SpriteRenderer;

SpriteRenderer *SpriteRenderer_Alloc();
bool SpriteRenderer_Init(SpriteRenderer *renderer);
void SpriteRenderer_SetProjection(SpriteRenderer *renderer, mat4 projection);
void SpriteRenderer_SetCamera(SpriteRenderer *renderer, vec3 position, float rotation, vec2 scale);
void SpriteRenderer_Draw(SpriteRenderer *renderer, Texture *sprite, vec2 position, vec2 size, GLfloat rotate, vec3 color);
void SpriteRenderer_Destroy(SpriteRenderer *renderer);
