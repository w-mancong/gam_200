#version 450 core

// layout (location = 0) in vec2 vTexCoord;

layout (location = 0) out vec4 fColor;

uniform sampler2D tex;

void main()
{
    fColor = vec4(1.0, 1.0, 1.0, 1.0);
}
