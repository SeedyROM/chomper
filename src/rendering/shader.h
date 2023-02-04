#pragma once

#include <stdbool.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <cglm/mat4.h>
#include <cglm/vec3.h>
#include <cglm/cglm.h>

typedef struct Shader
{
    GLuint program;
} Shader;

Shader *Shader_Alloc();

bool Shader_FromSource(Shader *shader, const char *vertex_source_path, const char *fragment_source_path);

void Shader_Use(Shader *shader);

void Shader_SetBool(Shader *shader, const char *name, bool value);
void Shader_SetInt(Shader *shader, const char *name, int value);
void Shader_SetFloat(Shader *shader, const char *name, float value);
void Shader_SetVec2(Shader *shader, const char *name, float x, float y);
void Shader_SetVec3(Shader *shader, const char *name, float x, float y, float z);
void Shader_SetVec4(Shader *shader, const char *name, float x, float y, float z, float w);
void Shader_SetMat4(Shader *shader, const char *name, mat4 *mat4);

void Shader_Destroy(Shader *shader);