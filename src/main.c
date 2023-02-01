#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#define TRY_ALLOC(ptr, type, count)                                  \
    type *ptr = malloc(sizeof(type) * count);                        \
    if (ptr == NULL)                                                 \
    {                                                                \
        fprintf(stderr, "Failed to allocate memory for %s\n", #ptr); \
        return ptr;                                                  \
    }

#define TRY_SDL(expr)                                        \
    if (!(expr))                                             \
    {                                                        \
        printf("SDL Error: %s %s\n", #expr, SDL_GetError()); \
        return false;                                        \
    }

typedef struct Renderer
{
    SDL_Window *window;
    SDL_GLContext gl_context;
} Renderer;

Renderer *Renderer_Alloc()
{
    TRY_ALLOC(renderer, Renderer, 1);
    return renderer;
}

bool Renderer_Init(Renderer *renderer, const char *title, int width, int height)
{
    // Initialize SDL
    TRY_SDL(SDL_Init(SDL_INIT_VIDEO) == 0);

    // Setup OpenGL attributes
    TRY_SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) == 0);
    TRY_SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2) == 0);
    TRY_SDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) == 0);
    TRY_SDL(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == 0);

    // Create the SDL window.
    renderer->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    TRY_SDL(renderer->window != NULL);

    // Create the OpenGL context.
    renderer->gl_context = SDL_GL_CreateContext(renderer->window);
    TRY_SDL(renderer->gl_context != NULL);

    // Setup OpenGL viewport.
    glViewport(0, 0, width, height);

    // Setup double buffering.
    TRY_SDL(SDL_GL_SetSwapInterval(1) == 0);

    return true;
}

void Renderer_Destroy(Renderer *renderer)
{
    SDL_GL_DeleteContext(renderer->gl_context);
    SDL_DestroyWindow(renderer->window);
    SDL_Quit();
}

void Renderer_Dealloc(Renderer *renderer)
{
    free(renderer);
}

typedef struct Shader
{
    GLuint program;
} Shader;

Shader *Shader_Alloc()
{
    TRY_ALLOC(shader, Shader, 1);
    return shader;
}

char *File_Open(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    TRY_ALLOC(buffer, char, length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
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

int main()
{
    // Create a renderer.
    Renderer *renderer = Renderer_Alloc();
    if (!Renderer_Init(renderer, "Hello World", 640, 480))
    {
        fprintf(stderr, "Failed to initialize renderer");
        return 1;
    }

    // Setup drawing
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // Draw wireframes.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Create the shader
    Shader *shader = Shader_Alloc();
    if (!Shader_FromSource(shader, "assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"))
    {
        fprintf(stderr, "Failed to create shader");
        return 1;
    }

    // Run the game loop
    bool close_requested = false;
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
            }
        }

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shader->program);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap our back buffer to the front
        SDL_GL_SwapWindow(renderer->window);
    }

    // Cleanup
    Shader_Dealloc(shader);
    Renderer_Destroy(renderer);
    Renderer_Dealloc(renderer);

    return 0;
}