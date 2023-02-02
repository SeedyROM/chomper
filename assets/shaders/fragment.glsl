#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, texCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor * vec4(vertexColor, 1.0);
} 