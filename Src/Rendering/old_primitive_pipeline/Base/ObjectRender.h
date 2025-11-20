//
// Created by Jingren Bai on 25-8-2.
//

#ifndef LEARNOPENGL_OBJECTRENDER_H
#define LEARNOPENGL_OBJECTRENDER_H

#include "cmake-build-release-visual-studio/vcpkg_installed/x64-windows/include/Eigen/Eigen"
#include "cmake-build-release-visual-studio/vcpkg_installed/x64-windows/include/glad/glad.h"
#include "cmake-build-release-visual-studio/vcpkg_installed/x64-windows/include/GLFW/glfw3.h"
#include <string>
#include "Rendering/Core/Vertex.h"
#include "Rendering/Core/Texture.h"
#include "Src/Rendering/Scene/Camera.hpp"
#include "Src/Rendering/Scene/Model.h"

class ObjectRender {
protected:
	std::string m_texturePath;
	std::vector<Vertex> vertices;
	unsigned int programID;
	unsigned int VAO, VBO, EBO;
	Eigen::Vector4f position;
	Camera m_camera;
public:
	bool isInitialized = false;
	virtual ~ObjectRender() = default;
	ObjectRender() = default;
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void destroy(){
		glDeleteProgram(programID);
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
		if(EBO){
			glDeleteBuffers(1, &EBO);
		}
	}

	void setCamera(Camera camera){
		this->m_camera = camera;
		LOG_INFO << "success set camera as\n" << m_camera.getViewMatrix() << '\n';
	}

	void setPosition(Eigen::Vector4f position){
		this->position = position;
		LOG_INFO << "success set position as\n" << position << '\n';
	}
};

#endif //LEARNOPENGL_OBJECTRENDER_H
