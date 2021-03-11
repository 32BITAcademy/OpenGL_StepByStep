#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inColors;

out vec3 vertColor;

uniform vec3 uniformPos;

void main()
{
	vec3 newPos = aPos/2 + uniformPos/2;
	vec3 dir = (newPos - vec3(0,0,0))/distance(vec3(0,0,0), newPos);

	gl_Position = vec4(newPos + dir/3.0f, 1.0f);
	vertColor = inColors;
}