//#version 450 core
//layout (location = 0) in vec4 aPos;
//out vec3 vColor;
////uniform mat4 model;
////uniform mat4 view;
////uniform mat4 projection;
//mat4 model;
//mat4 view;
//mat4 projection;
//
//void main() {
//    projection = mat4(1.0);
//    view = mat4(1.0);
//    model = mat4(1.0);
////    gl_Position = model * projection * view * aPos;
//    gl_Position = vec4(aPos.xyz, 32.0);
//    gl_PointSize = 3.0;
//    vColor = aPos.xyz;
//}
#version 450 core
layout (location = 0) in vec4 aPos;
out vec3 vColor;

void main() {
    // 简单缩放，把模拟空间 [0, 32] 映射到 [-1, 1] 左右
    vec3 p = aPos.xyz;

    // 把中心平移到 0 附近（根据你的边界大概是 0~32，可以微调）
    p -= vec3(16.0, 20.0, 16.0);  // 20.0 是大概的液面高度，可以自己试

    // 缩放到 [-1, 1] 区间
    p /= 20.0;   // 越大越“缩远”

    // 加一点旋转（比如绕 X 轴看一个俯视角）
    float angleX = 0.5; // 大概 30 度
    float c = cos(angleX);
    float s = sin(angleX);
    mat3 rotX = mat3(
    1, 0,  0,
    0, c, -s,
    0, s,  c
    );
    p = rotX * p;

    gl_Position = vec4(p, 1.0);
    gl_PointSize = 3.0;
    vColor = p;
}
