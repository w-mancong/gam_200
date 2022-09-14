#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in mat4 aInstanceModel;

uniform mat4 model, view, proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 0.0f, 1.0f);
}