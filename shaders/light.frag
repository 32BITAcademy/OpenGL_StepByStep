#version 330 core

out vec4 outColor;

uniform vec3 lightColor;

void main()
{
	outColor = vec4(lightColor, 1.0f);
}
