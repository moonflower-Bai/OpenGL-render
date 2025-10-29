//
// Created by Jingren Bai on 25-10-26.
//

#include "BallRender.h"

void BallRender::init(){
	LOG_INFO << "BallRender::init()";

	m_shader = Shader(m_vertexShaderPath, m_fragmentShaderPath);

	LOG_INFO << "Shader initialize finished.";

//	m_shader = phong;

	m_shader.use();
//	m_shader.setVec4("ourColor", color.x() / 255.0, color.y() / 255.0, color.z() / 255.0, color.w() / 255.0);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// get EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	Vertex *verticesArray = vertices.data(); // 注意这里直接是指针，在下面的函数中，不要加取地址
	/*
	 * parameter:
	 * target: 缓冲的类型
	 * 	GL_ARRAY_BUFFER: 顶点缓冲对象
	 * 	GL_ELEMENT_ARRAY_BUFFER: 元素缓冲对象
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

void BallRender::render(){
	//	glUseProgram(programID);
	m_shader.use();
	m_shader.setInt("ourTexture", 0);
	auto projection = m_camera.getProjectionMatrix();
	auto view = m_camera.getViewMatrix();
//	float angle = glfwGetTime() * 1.0f;
	auto model = Model::getTranslate(position) * Model::getRotation({0, {1.0f, 0.3f, 0.5f}});
	m_shader.setMat4("projection", projection);
	m_shader.setMat4("view", view);
	m_shader.setMat4("model", model);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

BallRender::BallRender(float radius, std::string vertexShaderPath, std::string fragmentShaderPath) {
	this->radius = radius;
	m_vertexShaderPath = vertexShaderPath;
	m_fragmentShaderPath = fragmentShaderPath;
	float step = 360.0f / 10;

	int stackCount = 180 / static_cast<int>(step) * 2;
	int sliceCount = 360 / static_cast<int>(step);

	auto toRadians = [](float degree) {
		float M_PI = 3.14159265358979323846f;
		return degree * M_PI / 180.0f;
	};

	for (int i = 0; i <= stackCount; ++i) {
		float phiDeg = i * step;
		float phi = toRadians(phiDeg);  // 极角 [0, π]

		for (int j = 0; j <= sliceCount; ++j) {
			float thetaDeg = j * step;
			float theta = toRadians(thetaDeg);  // 方位角 [0, 2π]

			float x = radius * std::sin(phi) * std::cos(theta);
			float y = radius * std::cos(phi);
			float z = radius * std::sin(phi) * std::sin(theta);

			float u = thetaDeg / 360.0f;
			float v = phiDeg / 180.0f;

			vertices.emplace_back(
					Eigen::Vector4f(0.5f, 0.5f, 0.5f, 1.0f),  // color
					Eigen::Vector4f(x, y, z, 1.0f),           // position
					Eigen::Vector2f(u, v)                     // texCoord
			);
		}
	}

	// 生成索引（构建三角形）
	for (int i = 0; i < stackCount; ++i) {
		for (int j = 0; j < sliceCount; ++j) {
			int topLeft     = i * (sliceCount + 1) + j;
			int topRight    = i * (sliceCount + 1) + (j + 1);
			int bottomLeft  = (i + 1) * (sliceCount + 1) + j;
			int bottomRight = (i + 1) * (sliceCount + 1) + (j + 1);

			// 上三角形
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);

			// 下三角形
			indices.push_back(topLeft);
			indices.push_back(bottomRight);
			indices.push_back(topRight);
		}
	}
}