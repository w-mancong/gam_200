#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;
layout (location = 3) in mat4 aInstanceMatrix;

layout (location = 0) out vec3 vColor;
layout (location = 1) out vec2 vTexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vColor = aColor;
	vTexCoord = aTexCoord;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 0.0, 1.0);

}