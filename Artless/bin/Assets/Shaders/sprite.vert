#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

layout (location = 0) out vec2 vTexCoord;

uniform mat4 model, view, proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 0.0f, 1.0f);
	vTexCoord = aTexCoord;
}