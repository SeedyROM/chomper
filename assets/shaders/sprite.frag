#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D spriteTexture;
uniform vec3 spriteColor;

void main()
{
    vec4 pixel = texture(spriteTexture, TexCoords);

    // If pixel is transparent, discard it
    if(pixel.a < 0.1)
        discard;

    color = vec4(spriteColor, 1.0) * pixel;
}