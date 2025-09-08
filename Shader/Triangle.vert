#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
//uniform mat4 mvp;
mat4 mvp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 ourColor;
out vec2 TexCoord;
void main() {
//    if(mvp == mat4(0.0)){
        mvp = projection * view * model;
//    }
    gl_Position = mvp * aPos;
    ourColor = aColor;
    TexCoord = aTexCoord;
}
