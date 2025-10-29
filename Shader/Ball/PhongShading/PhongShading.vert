#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
mat4 mvp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 ourColor;
out vec2 TexCoord;
out vec4 normalVector;
void main() {
    normalVector = normalize(aPos);
    mvp = projection * view * model;
    gl_Position = mvp * aPos;
    ourColor = aColor;
    TexCoord = aTexCoord;
}
