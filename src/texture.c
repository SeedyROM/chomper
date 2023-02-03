#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "common.h"

Texture *Texture_Alloc()
{
    TRY_ALLOC(texture, Texture, 1);
    texture->loaded = false;
    return texture;
}
bool Texture_Load(Texture *texture, const char *path)
{
    // Load the image data with stb_image.
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &texture->info.width, &texture->info.height, &texture->info.channels, 0);
    if (data == NULL)
    {
        fprintf(stderr, "Failed to load image: %s\n", path);
        fprintf(stderr, "Error: %s\n", stbi_failure_reason());
        return false;
    }

    // Mark the texture as loaded.
    texture->loaded = true;

    // Bind the texture.
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create the texture.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->info.width, texture->info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image.
    stbi_image_free(data);
    return true;
}
bool Texture_Unload(Texture *texture)
{
    glDeleteTextures(1, &texture->id);
    return true;
}
void Texture_Destroy(Texture *texture)
{
    free(texture);
}