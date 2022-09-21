//#version 330 core
//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec4 aTest;
//
//out vec3 fColor;
//
//void main()
//{
//    fColor = aColor;
//    gl_Position = vec4(aPos + vec2(aTest), 0.0, 1.0);
//}

#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 aInstanceMatrix;
//layout (location = 2) in vec4 a1stRow;
//layout (location = 3) in vec4 a2ndRow;
//layout (location = 4) in vec4 a3rdRow;
//layout (location = 5) in vec4 a4thRow;

out vec3 fColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    fColor = aColor;
    //mat4 model = mat4(vec4(a1stRow.x, a2ndRow.x, a3rdRow.x, a4thRow.x), 
    //                  vec4(a1stRow.y, a2ndRow.y, a3rdRow.y, a4thRow.y),
    //                  vec4(a1stRow.z, a2ndRow.z, a3rdRow.z, a4thRow.z),
    //                  vec4(a1stRow.w, a2ndRow.w, a3rdRow.w, a4thRow.w));

    //mat4 model = mat4(a1stRow, a2ndRow, a3rdRow, a4thRow);

    //gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 0.0, 1.0);

}