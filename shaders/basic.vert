#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inColors;
layout (location = 2) in vec2 inTexCoords;

out vec3 vertColor;
out vec2 texCoords;

uniform mat4 pvm;

void main()
{
	gl_Position = pvm * vec4(aPos, 1.0f);
	vertColor = inColors;
	texCoords = inTexCoords;
}