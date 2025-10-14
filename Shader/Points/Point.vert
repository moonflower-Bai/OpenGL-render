#version 330 core
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
mat4 mvp;
out vec4 v_color;

void main() {
    mvp = projection * view * model;
    gl_Position = mvp * a_position;
    v_color = a_color;
    gl_PointSize = 5.0;
}