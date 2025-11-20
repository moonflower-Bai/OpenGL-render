#version 450 core
out vec4 FragColor;
in vec3 vColor;
void main() {
//    float dist = length(gl_PointCoord - vec2(0.5));
//    if (dist > 0.5) discard;
//    gl_PointSize = 10.0;
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
//    FragColor = vec4(vColor, 1.0);
}
