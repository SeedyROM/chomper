#include "shader.h"

#include "common.h"

Shader *Shader_Alloc()
{
    TRY_ALLOC(shader, Shader, 1);
    return shader;
}

// TODO: Fix shader source warnings.
bool Shader_FromSource(Shader *shader, const char *vertex_source_path, const char *fragment_source_path)
{
    // Load the shader sources.
    const char *vertex_source = File_Open(vertex_source_path);
    const char *fragment_source = File_Open(fragment_source_path);

    // Create the vertex shader.
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    // Show the vertex shader compile log.
    GLint vertex_shader_log_length;
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &vertex_shader_log_length);
    if (vertex_shader_log_length > 0)
    {
        TRY_ALLOC(vertex_shader_log, char, vertex_shader_log_length);
        glGetShaderInfoLog(vertex_shader, vertex_shader_log_length, NULL, vertex_shader_log);
        printf("Vertex Shader Compile Log: %s\n", vertex_shader_log);
        free(vertex_shader_log);
    }

    // Create the fragment shader.
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    // Show the fragment shader compile log.
    GLint fragment_shader_log_length;
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &fragment_shader_log_length);
    if (fragment_shader_log_length > 0)
    {
        TRY_ALLOC(fragment_shader_log, char, fragment_shader_log_length);
        glGetShaderInfoLog(fragment_shader, fragment_shader_log_length, NULL, fragment_shader_log);
        printf("Fragment Shader Compile Log: %s\n", fragment_shader_log);
        free(fragment_shader_log);
    }

    // Create the shader program.
    shader->program = glCreateProgram();
    glAttachShader(shader->program, vertex_shader);
    glAttachShader(shader->program, fragment_shader);
    glLinkProgram(shader->program);

    // Delete the shaders.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Free the sources
    free((char *)vertex_source);
    free((char *)fragment_source);

    return true;
}

void Shader_Dealloc(Shader *shader)
{
    free(shader);
}