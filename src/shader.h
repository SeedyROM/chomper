#pragma once

#include <stdbool.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

typedef struct Shader
{
    GLuint program;
} Shader;

Shader *Shader_Alloc();
bool Shader_FromSource(Shader *shader, const char *vertex_source_path, const char *fragment_source_path);
void Shader_Dealloc(Shader *shader);