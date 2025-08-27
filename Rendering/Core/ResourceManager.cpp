//
// Created by Jingren Bai on 25-8-16.
//

#include "ResourceManager.h"

Shader &ResourceManager::getShader(std::string vertexPath, std::string fragmentPath) {
	Shader shader(vertexPath, fragmentPath);
	return shader;
}

Texture ResourceManager::getTexture(std::string texturePath) {
	return {texturePath};
}

Shader &ResourceManager::getDefaultShader() {
	Shader shader("Shaders/Triangle.vert", "Shaders/Triangle.frag");
	return shader;
}
