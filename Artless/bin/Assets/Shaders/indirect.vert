#version 460 core
#extension GL_ARB_bindless_texture  : enable
#extension ARB_gpu_shader_int64     : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in uint aDrawID;
layout (location = 4) in uint64_t aTexHandle;
layout (location = 5) in mat4 aInstanceMatrix;

layout (location = 0) out vec4 vColor;
layout (location = 1) out vec2 vTexCoord;
layout (location = 2) out flat uint64_t vTexHandle;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexHandle = aTexHandle;
    gl_Position = proj * view * aInstanceMatrix * vec4(aPos, 1.0);
}