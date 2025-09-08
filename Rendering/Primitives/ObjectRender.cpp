//
// Created by Jingren Bai on 25-8-2.
//

#include "ObjectRender.h"
void ObjectRender::destroy(){
	glDeleteProgram(programID);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if(EBO){
		glDeleteBuffers(1, &EBO);
	}
}

void ObjectRender::setTexturePath(std::string texturePath) {
	m_texturePath = texturePath;
}

void ObjectRender::setPosition(Eigen::Vector4f position) {
	this->position = position;
}
void ObjectRender::setCamera(Camera camera) {
	this->m_camera = camera;
}