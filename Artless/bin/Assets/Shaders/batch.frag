#version 450 core
layout (location = 0) in vec4 vColor;
layout (location = 1) in vec2 vTexCoord;

layout (location = 0) out vec4 fColor;

uniform sampler2D tex;

void main()
{
    fColor = vColor;
}
