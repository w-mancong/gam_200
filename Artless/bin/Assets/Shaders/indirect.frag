#version 460 core
#extension GL_ARB_bindless_texture  : require
#extension ARB_gpu_shader_int64     : enable

layout (location = 0) in vec4 vColor;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in flat uint64_t vTexHandle;

layout (location = 0) out vec4 fColor;

uniform sampler2D tex;

void main()
{
    // fColor = vColor;
    fColor = texture(sampler2D(uvec2(vTexHandle)), vTexCoord) * vColor;
}
