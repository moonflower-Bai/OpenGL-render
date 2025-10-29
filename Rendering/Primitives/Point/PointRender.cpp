//
// Created by Jingren Bai on 25-9-9.
//

#include "PointRender.h"
#include "Rendering/Assets/fluid/FluidSimulator.h"
#include <windows.h>

PointRender::PointRender(int particleNum, std::string vertexShaderPath, std::string fragmentShaderPath) {
	LOG_INFO << "PointRender::PointRender()";
	this->simulator = Simulator(particleNum);
	this->m_vertexShaderPath = vertexShaderPath;
	this->m_fragmentShaderPath = fragmentShaderPath;
	this->vertices = std::vector<Vertex>(particleNum);
	LOG_INFO << "PointRender::PointRender()";
}
void PointRender::setVertices(std::vector<Vertex> vertices) {
	this->vertices = vertices;
}
std::vector<Vertex> PointRender::getVertices() {
	return vertices;
}
void PointRender::init(){
	LOG_INFO << "PointRender::init()";
	m_shader = Shader(m_vertexShaderPath, m_fragmentShaderPath);
	m_shader.use();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), verticesArray, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void PointRender::update(){
//	Sleep(100);
	simulator.runPBF();
	vertices.clear();
	// 预分配空间以避免多次内存分配
	auto particles = simulator.getParticles();
//	int size = simulator.getParticleNums();
	for (int i = 0; i < particles.size(); i++){
//	for (int i = 0; i < size; i++){
		auto particle = particles[i];
		vertices.push_back(Vertex::particlesToVertex(particle));
		// 移除了重复的颜色和位置设置
		// 移除了耗时的日志输出
	}
//	LOG_INFO << "PointRender::update()" << vertices.back().position << '\n';
	// 更新顶点缓冲区数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void PointRender::render(){
	update();
	m_shader.use();
//	vertices.clear();
//	vertices.push_back({{1.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f},{0.0f, 0.0f}});
//	vertices.push_back({{1.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f},{0.0f, 0.0f}});
//	vertices.push_back({{1.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f},{0.0f, 0.0f}});

	glBindVertexArray(VAO);
	auto projection = m_camera.getProjectionMatrix();
	auto view = m_camera.getViewMatrix();
//	float angle = glfwGetTime() * 0.5f;
	float angle = 0;
//	auto model = Model::getTranslate(position) * Model::getRotation({angle, {1.0f, 0.3f, 0.5f}});
	auto model = Model::getTranslate(position);
//	auto projection = Eigen::Matrix4f::Identity();
//	auto view = Eigen::Matrix4f::Identity();
//	auto model = Eigen::Matrix4f::Identity();

	m_shader.setMat4("projection", projection);
	m_shader.setMat4("view", view);
	m_shader.setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, vertices.size());
}

void PointRender::destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(programID);
}