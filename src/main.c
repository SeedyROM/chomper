#include <stdio.h>

#include <cglm/vec3.h>

#include "window.h"
#include "texture.h"
#include "sprite_renderer.h"

int main()
{
    // Create a renderer.
    Window *window = Window_Alloc();
    if (!Window_Init(window, "Chomper v0.0.1", 1280, 720))
    {
        fprintf(stderr, "Failed to initialize renderer");
        return 1;
    }

    SpriteRenderer *sprite_renderer = SpriteRenderer_Alloc();
    if (!SpriteRenderer_Init(sprite_renderer))
    {
        fprintf(stderr, "Failed to initialize sprite renderer");
        return 1;
    }

    // Load the texture
    Texture *texture = Texture_Alloc();
    if (!Texture_Load(texture, "../assets/textures/awesome.png"))
    {
        fprintf(stderr, "Failed to load texture");
        return 1;
    }

    // Sprite position
    float rotation = 0.0f;

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

            case SDL_MOUSEMOTION:
                camera_position[0] = event.motion.x - 640.0f;
                camera_position[1] = event.motion.y - 360.0f;
                break;
            case SDL_MOUSEWHEEL:
                camera_scale[0] += event.wheel.y * 0.1f;
                camera_scale[1] += event.wheel.y * 0.1f;
                break;
            }
        }

        // Clear the screen
        Window_Clear(0.2f, 0.2f, 0.7f, 1.0f);

        // Draw the sprite
        rotation += 0.01f;

        // Update the camera
        SpriteRenderer_SetCamera(sprite_renderer, camera_position, 0.0f, (vec2){1.0f, 1.0f});

        // Fill the screen with sprites
        for (int y = 0; y < 720; y += 64)
        {
            for (int x = 0; x < 1280; x += 64)
            {
                SpriteRenderer_Draw(sprite_renderer, texture, (vec2){x, y}, (vec2){64, 64}, rotation, (vec3){1.0f, 1.0f, 1.0f});
            }
        }

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