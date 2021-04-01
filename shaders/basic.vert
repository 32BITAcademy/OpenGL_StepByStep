#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;
layout (location = 3) in vec3 inColors;

out vec3 vertColor;
out vec2 texCoords;
out vec3 vertNormal;
out vec3 FragPos;

uniform mat4 pv;
uniform mat4 model;

void main()
{
	vec4 vertPos = model * vec4(inPos, 1.0);
	gl_Position = pv * vertPos;
	vertColor = inColors;
	texCoords = inTexCoords;
	vertNormal = mat3(model)*inNormal;
	FragPos = vertPos.xyz;
}