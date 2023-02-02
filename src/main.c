#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

#include "renderer.h"
#include "common.h"
#include "texture.h"
#include "shader.h"

typedef struct Vertex
{
    float position[3];
    float color[3];
    float tex_coord[2];
} Vertex;

int main()
{
    // Create a renderer.
    Renderer *renderer = Renderer_Alloc();
    if (!Renderer_Init(renderer, "Chomper v0.0.1", 720, 720))
    {
        fprintf(stderr, "Failed to initialize renderer");
        return 1;
    }

    // Setup drawing
    Vertex vertices[] = {
        {
            .position = {0.5f, 0.5f, 0.0f},
            .color = {1.0f, 0.0f, 0.0f},
            .tex_coord = {1.0f, 1.0f},
        },
        {
            .position = {0.5f, -0.5f, 0.0f},
            .color = {0.0f, 1.0f, 0.0f},
            .tex_coord = {1.0f, 0.0f},
        },
        {
            .position = {-0.5f, -0.5f, 0.0f},
            .color = {0.0f, 0.0f, 1.0f},
            .tex_coord = {0.0f, 0.0f},
        },
        {
            .position = {-0.5f, 0.5f, 0.0f},
            .color = {1.0f, 1.0f, 0.0f},
            .tex_coord = {0.0f, 1.0f},
        },
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Create the vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create the vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Upload the vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create the element buffer object.
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Enable the vertex attribute
    glEnableVertexAttribArray(0);
    // Set the vertex attribute pointer
    glVertexAttribPointer(0, STRUCT_FIELD_SIZE(Vertex, position), GL_FLOAT, GL_FALSE, sizeof(Vertex), STRUCT_FIELD_OFFSET(Vertex, position));
    // Enable the vertex color attribute
    glEnableVertexAttribArray(1);
    // Set the vertex color attribute pointer
    glVertexAttribPointer(1, STRUCT_FIELD_SIZE(Vertex, color), GL_FLOAT, GL_FALSE, sizeof(Vertex), STRUCT_FIELD_OFFSET(Vertex, color));
    // Enable the vertex texture coordinate attribute
    glEnableVertexAttribArray(2);
    // Set the vertex texture coordinate attribute pointer
    glVertexAttribPointer(2, STRUCT_FIELD_SIZE(Vertex, tex_coord), GL_FLOAT, GL_FALSE, sizeof(Vertex), STRUCT_FIELD_OFFSET(Vertex, tex_coord));

    // Create the shader
    Shader *shader = Shader_Alloc();
    if (!Shader_FromSource(shader, "../assets/shaders/vertex.glsl", "../assets/shaders/fragment.glsl"))
    {
        fprintf(stderr, "Failed to create shader");
        return 1;
    }

    // Load the texture
    Texture *texture = Texture_Alloc();
    if (!Texture_Load(texture, "../assets/textures/awesome.png"))
    {
        fprintf(stderr, "Failed to load texture");
        return 1;
    }

    // Run the game loop
    bool close_requested = false;
    bool show_wireframe = false;
    while (!close_requested)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                close_requested = true;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    close_requested = true;
                    break;

                case SDLK_k:
                    show_wireframe = !show_wireframe;
                    glPolygonMode(GL_FRONT_AND_BACK, show_wireframe ? GL_LINE : GL_FILL);
                    break;
                }
                break;
            }
        }

        // Clear the screen to black
        glClearColor(0.2f, 0.2f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shader->program);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap our back buffer to the front
        SDL_GL_SwapWindow(renderer->window);
    }

    // Cleanup
    Shader_Dealloc(shader);

    Texture_Unload(texture);
    Texture_Dealloc(texture);

    Renderer_Destroy(renderer);
    Renderer_Dealloc(renderer);

    return 0;
}