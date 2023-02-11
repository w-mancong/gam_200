#version 430 core
#extension GL_ARB_bindless_texture  : enable
#extension ARB_gpu_shader_int64     : enable

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in uint64_t aTexHandle;
layout (location = 4) in uint aDrawID;
layout (location = 5) in mat4 instanceMatrix;
layout (location = 6) in uint64_t UI;

layout (location = 0) out vec2 vTexCoord;
layout (location = 1) flat out uint vIndex;
layout (location = 2) out vec4 vColor;
layout (location = 3) flat out uint64_t vTexHandle;

uniform mat4 view, proj, ortho;

void main(void)
{
  vTexCoord = aTexCoord;
  vIndex = uint(instanceMatrix[3][3]);
  mat4 model = instanceMatrix;
  model[3][3] = 1.0;
  vColor = aColor;
  vTexHandle = aTexHandle;

  if(UI == 0)
  {
    gl_Position = proj * view * model * vec4(aPosition, 1.0);
  }
  else
  {
    gl_Position = ortho * model * vec4(aPosition, 1.0);
  }
}