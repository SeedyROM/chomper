#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <cglm/vec3.h>

#include "rendering/rendering.h"

typedef struct Sprite
{
    Texture *texture;
    vec3 position;
    vec3 size;
    vec3 color;
    vec3 scale;
    float rotation;
} Sprite;

void Sprite_FromTexture(Sprite *sprite, Texture *texture, vec3 position, vec3 color, float rotation)
{
    sprite->texture = texture;
    memcpy(sprite->position, position, sizeof(vec3));
    memcpy(sprite->size, (vec3){texture->info.width, texture->info.height, 1.0f}, sizeof(vec3));
    memcpy(sprite->color, color, sizeof(vec3));
    sprite->rotation = rotation;
}

void Sprite_SetScale(Sprite *sprite, vec3 scale)
{
    memcpy(sprite->scale, scale, sizeof(vec3));
}

void Sprite_Draw(Sprite *sprite, SpriteRenderer *renderer)
{
    vec3 scaled_size = {sprite->scale[0] * sprite->size[0], sprite->scale[1] * sprite->size[1], 1.0f};
    SpriteRenderer_Draw(renderer, sprite->texture, sprite->position, scaled_size, sprite->rotation, sprite->color);
}

int main()
{
    // Create a window and gl context.
    Window *window = Window_Alloc();
    if (!Window_Init(window, "Chomper v0.0.1", 1280, 720))
    {
        fprintf(stderr, "Failed to initialize renderer\n");
        return 1;
    }

    // Create a sprite renderer.
    SpriteRenderer *sprite_renderer = SpriteRenderer_Alloc();
    if (!SpriteRenderer_Init(sprite_renderer, window))
    {
        fprintf(stderr, "Failed to initialize sprite renderer\n");
        return 1;
    }

    // Load a texture
    Texture *texture = Texture_Alloc();
    if (!Texture_Load(texture, "../assets/textures/awesome.png"))
    {
        fprintf(stderr, "Failed to load texture\n");
        return 1;
    }

    // Create a sprite.
    Sprite sprite;
    Sprite_FromTexture(&sprite, texture, (vec3){0.0f, 0.0f, 0.0f}, (vec3){1.0f, 1.0f, 1.0f}, 0.0f);
    Sprite_SetScale(&sprite, (vec3){0.33f, 0.33f, 1.0f});

    // Create another sprite.
    Sprite sprite2;
    Sprite_FromTexture(&sprite2, texture, (vec3){200.0f, 200.0f, 0.0f}, (vec3){1.0f, 1.0f, 1.0f}, 0.0f);
    Sprite_SetScale(&sprite2, (vec3){0.33f, 0.33f, 1.0f});

    // Camera position
    vec3 camera_position = {0.0f, 0.0f, 0.0f};
    vec2 camera_scale = {1.0f, 1.0f};

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

        // Get key states.
        const Uint8 *key_states = SDL_GetKeyboardState(NULL);

        // Update camera position.
        if (key_states[SDL_SCANCODE_W])
        {
            camera_position[1] += 10.0f;
        }
        if (key_states[SDL_SCANCODE_S])
        {
            camera_position[1] -= 10.0f;
        }
        if (key_states[SDL_SCANCODE_A])
        {
            camera_position[0] -= 10.0f;
        }
        if (key_states[SDL_SCANCODE_D])
        {
            camera_position[0] += 10.0f;
        }

        // Update the camera zoom.
        if (key_states[SDL_SCANCODE_Q])
        {
            camera_scale[0] -= 0.01f;
            camera_scale[1] -= 0.01f;
        }
        if (key_states[SDL_SCANCODE_E])
        {
            camera_scale[0] += 0.01f;
            camera_scale[1] += 0.01f;
        }

        // Update sprite position.
        if (key_states[SDL_SCANCODE_UP])
        {
            sprite.position[1] -= 10.0f;
        }
        if (key_states[SDL_SCANCODE_DOWN])
        {
            sprite.position[1] += 10.0f;
        }
        if (key_states[SDL_SCANCODE_LEFT])
        {
            sprite.position[0] -= 10.0f;
        }
        if (key_states[SDL_SCANCODE_RIGHT])
        {
            sprite.position[0] += 10.0f;
        }

        // Clear the screen
        Window_Clear(0.2f, 0.2f, 0.7f, 1.0f);

        // Update the camera
        SpriteRenderer_SetCamera(sprite_renderer, camera_position, 0, camera_scale);

        // Draw the sprites
        Sprite_Draw(&sprite2, sprite_renderer);
        Sprite_Draw(&sprite, sprite_renderer);

        // Swap our back buffer to the front
        Window_Swap(window);
    }

    // Cleanup
    // Shader_Destroy(shader);

    Texture_Unload(texture);
    Texture_Destroy(texture);

    Window_Deinit(window);
    Window_Destroy(window);

    return 0;
}