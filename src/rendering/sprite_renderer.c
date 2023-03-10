#include "sprite_renderer.h"

#include "common.h"

static const float SPRITE_QUAD_VERTICES[] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f};

SpriteRenderer *SpriteRenderer_Alloc()
{
    TRY_ALLOC(renderer, SpriteRenderer, 1);
    return renderer;
}

bool SpriteRenderer_Init(SpriteRenderer *renderer, Window *window)
{
    // Set the initial window size.
    int width, height;
    SDL_GetWindowSize(window->window, &width, &height);
    SpriteRenderer_SetSize(renderer, (vec2){(float)width, (float)height});

    // Create the view matrix
    glm_mat4_identity(renderer->view);

    // Create VAO
    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    // Create VBO
    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SPRITE_QUAD_VERTICES), SPRITE_QUAD_VERTICES, GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0);

    // Create shader
    renderer->shader = Shader_Alloc();
    if (!Shader_FromSource(renderer->shader, "../assets/shaders/sprite.vert", "../assets/shaders/sprite.frag"))
    {
        fprintf(stderr, "Failed to initialize sprite shader");
        return false;
    }

    return true;
}

void SpriteRenderer_SetSize(SpriteRenderer *renderer, vec2 size)
{
    glm_ortho(0.0f, size[0], size[1], 0.0f, -1.0f, 1.0f, renderer->projection);
}

void SpriteRenderer_SetCamera(SpriteRenderer *renderer, vec3 position, float rotation, vec2 scale)
{
    glm_mat4_identity(renderer->view);
    // Translate
    glm_translate(renderer->view, position);
    // Rotate around the center of the screen
    glm_translate(renderer->view, (vec3){renderer->size[0] / 2.0f, renderer->size[1] / 2.0f, 0.0f});
    glm_rotate_z(renderer->view, rotation, renderer->view);
    glm_translate(renderer->view, (vec3){-renderer->size[0] / 2.0f, -renderer->size[1] / 2.0f, 0.0f});
    // Scale
    glm_scale(renderer->view, scale);
}

void SpriteRenderer_Draw(SpriteRenderer *renderer, Texture *texture, vec2 position, vec2 size, float rotate, vec3 color)
{
    // Prepare transformations
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3){position[0], position[1], 0.0f});
    glm_translate(model, (vec3){0.5f * size[0], 0.5f * size[1], 0.0f});
    glm_rotate(model, rotate, (vec3){0.0f, 0.0f, 1.0f});
    glm_translate(model, (vec3){-0.5f * size[0], -0.5f * size[0], 0.0f});
    glm_scale(model, (vec3){size[0], size[1], 1.0f});

    // Calculate the model-view-projection matrix on the CPU.
    mat4 mvp;
    glm_mat4_mul(renderer->projection, renderer->view, mvp);
    glm_mat4_mul(mvp, model, mvp);

    // Enable transparent textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup the shader and uniforms
    Shader_Use(renderer->shader);
    Shader_SetMat4(renderer->shader, "mvp", &mvp);
    Shader_SetVec3(renderer->shader, "spriteColor", color[0], color[1], color[2]);

    // Bind and draw the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glBindVertexArray(renderer->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // Disable transparent textures
    glDisable(GL_BLEND);
}