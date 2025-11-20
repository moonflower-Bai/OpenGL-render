//
// Created by Jingren Bai on 25-8-10.
//

#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "utils/getProgramPath.h"

class Texture {
private:
	unsigned int texture;
	std::string texturePath;
	void initTexture();
public:
	Texture(std::string path);
	Texture(Texture &&texture) noexcept ;
	Texture(const Texture &texture) = delete;
	Texture& operator=(Texture &&texture) noexcept ;
	Texture& operator=(const Texture &texture) = delete;
	bool empty() const;
	Texture();
	~Texture();

	void bind();
};


#endif //LEARNOPENGL_TEXTURE_H
