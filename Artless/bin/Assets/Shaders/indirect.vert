#version 430 core
#extension GL_ARB_bindless_texture  : enable
#extension ARB_gpu_shader_int64     : enable

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in uint aDrawID;
layout (location = 3) in vec4 aColor;
layout (location = 4) in uint64_t aTexHandle;
layout (location = 5) in mat4 instanceMatrix;

layout (location = 0) out vec2 vTexCoord;
layout (location = 1) flat out uint vDrawID;
layout (location = 2) out vec4 vColor;
layout (location = 3) flat out uint64_t vTexHandle;

uniform mat4 view, proj;

void main(void)
{
  vTexCoord = aTexCoord;
  vDrawID = aDrawID;
  vColor = aColor;
  vTexHandle = aTexHandle;
  gl_Position = proj * view * instanceMatrix * vec4(aPosition, 1.0);
}