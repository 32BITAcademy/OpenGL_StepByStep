#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;
layout (location = 3) in vec3 inColors;

uniform mat4 pv;
uniform mat4 model;

void main()
{
	gl_Position = pv * model * vec4(inPos, 1.0);
}