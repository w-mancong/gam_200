#version 450 core
layout (location = 0) out vec4 fColor;

uniform vec4 color = vec4(0.0f, 1.0f, 0.0f, 1.0f);

void main()
{
	fColor =  color;
}