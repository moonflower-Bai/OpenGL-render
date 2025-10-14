//
// Created by Jingren Bai on 25-8-2.
//

#ifndef LEARNOPENGL_OBJECTRENDER_H
#define LEARNOPENGL_OBJECTRENDER_H

#include <Eigen/Eigen>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Rendering/Core/Vertex.h"
#include "Rendering/Core/Texture.h"
#include "Rendering/Scene/Camera.hpp"
#include "Rendering/Scene/Model.h"

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
	virtual void destroy();
	void setCamera(Camera camera);
	void setPosition(Eigen::Vector4f position);
};


#endif //LEARNOPENGL_OBJECTRENDER_H
