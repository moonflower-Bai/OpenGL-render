#include <Eigen/Eigen>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils/log.cpp"
#include "Rendering/Primitives/TriangleRendering.h"
#include "Rendering/Pipeline/MainRenderThread.h"
#include "Extern/stb_image.h"
#include "utils/getProgramPath.h"
#include "Rendering/Scene/Camera.hpp"

int width = 800, height = 600;
int main(){
	MainRenderThread::init(width, height);
//	Eigen::Vector4f tianyi(0x66 / 255.0, 0xcc / 255.0, 0xff / 255.0, 1.0f);
//	Eigen::Vector4f aling(0xee / 255.0, 0x00 / 255.0, 0x00 / 255.0, 1.0f);
//	std::vector m_vertices = {
//		Vertex(tianyi, Eigen::Vector3f(-0.5, -0.5, 0.0), Eigen::Vector2f(0.0, 0.0)),
//		Vertex(tianyi, Eigen::Vector3f(0.5, -0.5, 0.0), Eigen::Vector2f(0.0, 1.0)),
//		Vertex(tianyi, Eigen::Vector3f(0.0, 0.5, 0.0), Eigen::Vector2f(1.0, 0.5))
//	};
//	std::vector<Vertex> vertices_2 = {
//		Vertex(aling, Eigen::Vector3f(1, 0, 0), Eigen::Vector2f(1, 0)),
//		Vertex(aling, Eigen::Vector3f(0, 1, 0), Eigen::Vector2f(0, 1)),
//		Vertex(aling, Eigen::Vector3f(1, 1, 0), Eigen::Vector2f(1, 1))
//	};
	float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	auto toVertices =[](const float* data, size_t size, const Eigen::Vector4f& color) {
		std::vector<Vertex> result;
		for (size_t i = 0; i < size; i += 5) {
			Eigen::Vector3f position(data[i], data[i+1], data[i+2]);
			Eigen::Vector2f texCoords(data[i+3], data[i+4]);
			result.emplace_back(color, position, texCoords);
		}
		return result;
	};
	std::vector<Eigen::Vector3d> positions = {
			Eigen::Vector3d( 0.0,  0.0,  0.0),
			Eigen::Vector3d( 2.0,  5.0, -15.0),
			Eigen::Vector3d(-1.5, -2.2, -2.5),
			Eigen::Vector3d(-3.8, -2.0, -12.3),
			Eigen::Vector3d( 2.4, -0.4, -3.5),
			Eigen::Vector3d(-1.7,  3.0, -7.5),
			Eigen::Vector3d( 1.3, -2.0, -2.5),
			Eigen::Vector3d( 1.5,  2.0, -2.5),
			Eigen::Vector3d( 1.5,  0.2, -1.5),
			Eigen::Vector3d(-1.3,  1.0, -1.5)
	};
	std::vector<Vertex> m_vertices = toVertices(vertices, sizeof(vertices) / sizeof(float), Eigen::Vector4f(0.5, 0.5, 0.5, 1.0));
	std::string projectRoot = getProjectPath();
	std::string vertexShaderPath = projectRoot + "/Shader/Triangle.vert";
	std::string fragmentShaderPath = projectRoot + "/Shader/Triangle.frag";
	std::string texturePath = "/Texture/wall.jpg";
//	Camera mainCamera(Eigen::Vector3d(0, 0, 3), Eigen::Vector3d(0, 0, -3), Eigen::Vector3d(0, 1, 0));
//	auto projection = mainCamera.getProjectionMatrix(45.0, (float)width / (float)height, 0.1, 100.0);
//	auto model = Eigen::Matrix4d::Identity();
//	auto view = mainCamera.getViewMatrix();
	auto projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	auto view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, -3), glm::vec3(0, 1, 0));
	auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	auto tmp = (projection * view * model);
	Eigen::Matrix4d mvp;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			mvp(i, j) = tmp[i][j];
		}
	}
	LOG_INFO << "projection: \n" << projection;
	LOG_INFO << "view: \n" << view;
	LOG_INFO << "model: \n" << model;

	for(int i = 0; i < positions.size(); i++){
		auto pos = mvp * Eigen::Vector4d(m_vertices[i].position[0], m_vertices[i].position[1], m_vertices[i].position[2], 1.0);
		auto posf = Eigen::Vector3f(float(pos[0]), float(pos[1]), float(pos[2]));
		m_vertices[i].position = posf;
	}
	auto tmpT = std::make_unique<TriangleRendering>(m_vertices, vertexShaderPath, fragmentShaderPath, texturePath);
	MainRenderThread::add(std::move(tmpT));
//	auto tgr1 = std::make_unique<TriangleRendering>(m_vertices, vertexShaderPath, fragmentShaderPath, texturePath);
//	MainRenderThread::add(std::move(tgr1));
//	auto tgr2 = std::make_unique<TriangleRendering>(vertices_2, vertexShaderPath, fragmentShaderPath, texturePath);
//	MainRenderThread::add(std::move(tgr2));

	MainRenderThread::instance().waitForExit();
	return 0;
}