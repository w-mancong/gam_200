#version 430 core
#extension GL_ARB_bindless_texture  : require
#extension ARB_gpu_shader_int64     : enable

layout (location = 0) in vec2 vTexCoord;
layout (location = 1) flat in uint vDrawID;
layout (location = 2) in vec4 vColor;
layout (location = 3) flat in uint64_t vTexHandle;

layout (location = 0) out vec4 fColor;

void main(void)
{
    fColor = texture(sampler2DArray(uvec2(vTexHandle)), vec3(vTexCoord, 0.0)) * vColor;
    // fColor = texture(sampler2D(uvec2(vTexHandle)), vTexCoord) * vColor;
    
    // fColor = texture(textureArray, vec3(uv.x, uv.y, drawID) );
}