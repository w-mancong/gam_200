#version 430 core
layout (location = 0) in vec2 vTexCoord;

layout (location = 0) out vec4 fColor;

// texture samplers
uniform sampler2DArray tex;

void main()
{
    fColor = texture(tex, vec3(vTexCoord, 2.0));
	// FragColor = texture(tex, vec3(vTexCoord, 0.0)) * vec4(vColor, 1.0);
}