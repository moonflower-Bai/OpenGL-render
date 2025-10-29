#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 cameraPosition;

out vec4 ourColor;
out vec2 TexCoord;

vec4 GouraudShading(vec3 normalVector, vec3 cameraPos){
    vec3 lightPosition = vec3(5.0, 5.0, 5.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 ambientColor = vec3(0.1, 0.1, 0.1);

    vec3 norm = normalize(normalVector);
    vec3 lightDir = normalize(lightPosition - vec3(0.0));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(cameraPos - vec3(0.0));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;

    vec3 color = ambientColor + diffuse + specular;
    return vec4(color, 1.0);
}

void main() {
    mat4 mvp = projection * view * model;
    gl_Position = mvp * aPos;

    vec3 normal = normalize(aPos.xyz);
    vec3 camPos = cameraPosition.xyz;

    ourColor = GouraudShading(normal, camPos) * aColor;
    TexCoord = aTexCoord;
}
