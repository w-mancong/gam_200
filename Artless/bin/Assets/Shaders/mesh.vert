#version 450 core
layout (location = 0) in vec2 aPos;

uniform mat4 scale, rotate, translate, view, proj;

void main()
{
	gl_Position = proj * view * translate * rotate * scale * vec4(aPos, 0.0f, 1.0f);
}