#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in uint drawid;
layout (location = 3) in vec4 aColor;
layout (location = 4) in mat4 instanceMatrix;

layout (location = 0) out vec2 uv;
layout (location = 1) flat out uint drawID;
layout (location = 2) out vec4 vColor;

uniform mat4 view, proj;

void main(void)
{
  uv = texCoord;
  drawID = drawid;
  vColor = aColor;
  gl_Position = proj * view * instanceMatrix * vec4(position, 1.0);
}