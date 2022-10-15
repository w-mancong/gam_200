#version 430 core
layout (location = 0) in vec2 uv;
layout (location = 1) flat in uint drawID;
layout (location = 2) in vec4 vColor;

layout (location = 0) out vec4 fColor;
layout (binding  = 0) uniform sampler2DArray textureArray;

void main(void)
{
    fColor = vColor;
    
    // fColor = texture(textureArray, vec3(uv.x, uv.y, drawID) );
}