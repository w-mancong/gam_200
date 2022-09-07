#version 450 core
layout (location = 0) in vec2 	aPos;
layout (location = 1) in vec2 	aTexCoord;
layout (location = 2) in float 	aTexIndex;
layout (location = 3) in mat4 	aInstanceMatrix;

layout (location = 0) out vec2 	vTexCoord;
layout (location = 1) out float vTexIndex;

uniform mat4 view, proj;

void main()
{
	gl_Position = proj * view * aInstanceMatrix * vec4(aPos, 0.0f, 1.0f);
	vTexCoord = aTexCoord;
	vTexIndex = aTexIndex;
}