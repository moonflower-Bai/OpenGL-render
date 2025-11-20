//
// Created by Jingren Bai on 25-8-16.
//

#ifndef LEARNOPENGL_RESOURCEMANAGER_H
#define LEARNOPENGL_RESOURCEMANAGER_H

#include <unordered_map>
#include <string>
#include "src/Shader/Shader.h"
#include "Texture.h"

class ResourceManager {
private:
	static inline std::unordered_map<std::string, Shader> shaders;
	static inline std::unordered_map<std::string, Texture> textures;
	static inline std::string projectRoot;
public:
	static void iniltialize(std::string projectRoot);
	static void destroy();

	static Shader& getShader(std::string vertexPath, std::string fragmentPath);
	static Texture getTexture(std::string texturePath);

	static Shader& getDefaultShader();
};


#endif //LEARNOPENGL_RESOURCEMANAGER_H
