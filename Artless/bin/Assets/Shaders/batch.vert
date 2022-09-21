#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec4 vColor;
layout (location = 1) out vec2 vTexCoord;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    gl_Position = proj * view * vec4(aPos, 1.0);
}