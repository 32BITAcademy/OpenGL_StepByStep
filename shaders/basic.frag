#version 330 core

in vec3 vertColor;
in vec2 texCoords;
out vec4 outColor;

uniform sampler2D ourTexture;

void main()
{
    outColor = texture(ourTexture, texCoords) * vec4(vertColor, 1.0f);
}
