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

void ObjectRender::setPosition(Eigen::Vector4f position) {
	this->position = position;
	LOG_INFO << "success set position as\n" << position << '\n';
}
void ObjectRender::setCamera(Camera camera) {
	this->m_camera = camera;
	LOG_INFO << "success set camera as\n" << m_camera.getViewMatrix() << '\n';
}