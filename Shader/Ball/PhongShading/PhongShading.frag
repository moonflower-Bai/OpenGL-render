#version 330 core
in vec4 ourColor;
in vec2 TexCoord;
in vec4 normalVector;
out vec4 FragColor;

uniform vec4 cameraPosition;

vec4 phongShading(vec4 normalVector, vec4 cameraPosition){
    vec4 lightPosition = vec4(5.0, 5.0, 5.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 ambientColor = vec4(0.1, 0.1, 0.1, 1.0);

    vec4 norm = normalize(normalVector);
    vec4 lightDir = normalize(lightPosition - vec4(0.0, 0.0, 0.0, 1.0));
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    vec4 viewDir = normalize(cameraPosition - vec4(0.0, 0.0, 0.0, 1.0));
    vec4 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = spec * lightColor;

    return ambientColor + diffuse + specular;
}

void main(){
    FragColor = phongShading(normalVector, cameraPosition) * ourColor;
}

