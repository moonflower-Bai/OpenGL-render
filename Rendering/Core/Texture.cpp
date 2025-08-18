//
// Created by Jingren Bai on 25-8-10.
//

#include "Texture.h"

Texture::Texture(std::string path) {
	std::string root;
	root = getProjectPath();
	texturePath = root + path;
	initTexture();
}
void Texture::initTexture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	LOG_INFO << "Loading texture from path: " << texturePath << ". ";
	if(data){
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		LOG_ERROR << "Failed to load texture";
	}
	stbi_image_free(data);
}
Texture::Texture(){
	texture = 0;
	texturePath = "";
	LOG_WARNING << "Using empty texture path at default constructor, maybe you should check your texture initialization";
}
Texture::Texture(Texture &&texture) noexcept {
	texturePath = std::move(texture.texturePath);
	this->texture = texture.texture;
	texture.texture = 0;
}
Texture& Texture::operator=(Texture &&texture) noexcept {
	if(this == &texture) return *this;
	if(this->texture != 0){
		glDeleteTextures(1, &this->texture);
	}
	texturePath = std::move(texture.texturePath);
	this->texture = texture.texture;
	texture.texture = 0;
	return *this;
}
Texture::~Texture() {
	if(!texture) return;
	glDeleteTextures(1, &texture);
}
void Texture::bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}