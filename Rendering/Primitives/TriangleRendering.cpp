//
// Created by Jingren Bai on 25-7-15.
//

#include "TriangleRendering.h"

#include <utility>
#include "utils/log.cpp"

std::vector<Vertex> TriangleRendering::getVertices() {
	return vertices;
}
void TriangleRendering::init() {
	LOG_INFO << "TriangleRendering::init()";
	m_shader = Shader(m_vertexShaderPath, m_fragmentShaderPath);
	m_shader.use();
//	m_shader.setVec4("ourColor", color.x() / 255.0, color.y() / 255.0, color.z() / 255.0, color.w() / 255.0);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	LOG_INFO << "verticesArray: \n" << vertices << '\n';
	m_texture = Texture(m_texturePath);

	Vertex *verticesArray = vertices.data(); // 注意这里直接是指针，在下面的函数中，不要加取地址
	/*
	 * parameter:
	 * target: 缓冲的类型
	 * 	GL_ARRAY_BUFFER: 顶点缓冲对象
	 * size: 声明数组的大小
	 * data: 指向数组的指针
	 * usage: 告诉OpenGL如何使用这个缓冲区对象；
	 * 	GL_STATIC_DRAW: 缓冲数据不会或几乎不会改变
	 * 	GL_DYNAMIC_DRAW: 缓冲数据会被改变很多
	 * 	GL_STREAM_DRAW: 缓冲数据每次使用一次就丢弃
	 * 缓冲对象数据
	 */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), verticesArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TriangleRendering::render() {
//	glUseProgram(programID);
	m_shader.use();
	m_texture.bind();
	m_shader.setInt("ourTexture", 0);
	auto projection = m_camera.getProjectionMatrix();
	auto view = m_camera.getViewMatrix();
//	float angle = glfwGetTime() * 1.0f;
	auto model = Model::getTranslate(position) * Model::getRotation({0, {1.0f, 0.3f, 0.5f}});
	m_shader.setMat4("projection", projection);
	m_shader.setMat4("view", view);
	m_shader.setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

TriangleRendering::TriangleRendering(const std::vector<Vertex> vertices, std::string vertexShaderPath,
									 std::string fragmentShaderPath, std::string texturePath){
	setVertices(vertices);
	setVertexShaderPath(vertexShaderPath);
	setFragmentShaderPath(fragmentShaderPath);
	setTexturePath(texturePath);
}

TriangleRendering::TriangleRendering() = default;
void TriangleRendering::setVertices(std::vector<Vertex> vertices) {
	this->vertices = vertices;
}
void TriangleRendering::destroy(){
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(programID);
}
void TriangleRendering::setVertexShaderPath(std::string vertexShaderPath) {
	this->m_vertexShaderPath = std::move(vertexShaderPath);
}
void TriangleRendering::setFragmentShaderPath(std::string fragmentShaderPath) {
	this->m_fragmentShaderPath = std::move(fragmentShaderPath);
}

Shader* TriangleRendering::getShader() {
	return &m_shader;
}

void TriangleRendering::setTexturePath(std::string texturePath) {
	m_texturePath = texturePath;
}
