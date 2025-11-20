#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
//out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
//    Normal = mat3(transpose(inverse(model))) * aPos; // 如果模型有缩放，需要法线矩阵
    // 对于单位球且无非均匀缩放，可以直接用 aPos 当法线
//    Normal = normalize(Normal);
    Normal = normalize(aPos);

    gl_Position = projection * view * worldPos;
//    ourColor = aColor;
    TexCoord = aTexCoord;
}