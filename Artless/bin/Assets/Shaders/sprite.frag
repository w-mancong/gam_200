#version 450 core

layout (location = 0) in vec2  vTexCoord;
layout (location = 1) in float vTexIndex;

layout (location = 0) out vec4 fColor;

uniform sampler2D texture1;
uniform vec4 color;

void main()
{
	fColor = texture(texture1, vTexCoord) * color;
}