#version 330 core

in vec3 vertColor;
in vec2 texCoords;
out vec4 outColor;

uniform sampler2D ourTexture;
uniform bool wireframeMode;

void main()
{
    if (wireframeMode)
        outColor = vec4(vertColor, 1.0f);
    else
        outColor = texture(ourTexture, texCoords) * vec4(vertColor, 1.0f);
}
