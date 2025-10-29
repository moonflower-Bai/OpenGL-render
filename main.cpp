#include <Eigen/Eigen>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils/log.cpp"
#include "Rendering/Primitives/Triangle/TriangleRender.h"
#include "Rendering/Pipeline/MainRenderThread.h"
#include "Extern/stb_image.h"
#include "utils/getProgramPath.h"
#include "Rendering/Scene/Camera.hpp"
#include "Rendering/Primitives/Point/PointRender.h"
#include "Rendering/Primitives/Ball/BallRender.h"

#pragma comment(linker, "/STACK:1073741824")

int width = 1200, height = 600;
int lighting_main();
int pst_main();

int main(){
	return lighting_main();
//	return pst_main();
}
int lighting_main(){
	MainRenderThread::init(width, height);
	Camera camera({0, 0, 3.0f}, {0.0f, 0.0f, 0.0f}, {0, 1, 0},
		   45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

	std::string projectRoot = getProjectPath();
	std::string PhongShader[] = {
			projectRoot + "/Shader/Ball/PhongShading/PhongShading.frag",
			projectRoot + "/Shader/Ball/PhongShading/PhongShading.vert"
	};

	std::string GouraudShader[] = {
			projectRoot + "/Shader/Ball/GouraudShading/GouraudShading.frag",
			projectRoot + "/Shader/Ball/GouraudShading/GouraudShading.vert"
	};

	auto position1 = Eigen::Vector4f(-1, 0, 0, 1);
	auto position2 = Eigen::Vector4f(1, 0, 0, 1);
	auto phong = std::make_unique<BallRender>(1.0f, PhongShader[1], PhongShader[0]);
	auto gouraud = std::make_unique<BallRender>(1.0f, GouraudShader[1], GouraudShader[0]);
	phong->setPosition(position1);
	phong->setCamera(camera);
	gouraud->setPosition(position2);
	gouraud->setCamera(camera);
	MainRenderThread::add(std::move(gouraud));
	MainRenderThread::add(std::move(phong));
	MainRenderThread::instance().waitForExit();
	return 0;
}

int pst_main(){
	MainRenderThread::init(width, height);
//	Eigen::Vector4f tianyi(0x66 / 255.0, 0xcc / 255.0, 0xff / 255.0, 1.0f);
//	Eigen::Vector4f aling(0xee / 255.0, 0x00 / 255.0, 0x00 / 255.0, 1.0f);
//	std::vector m_vertices = {
//		Vertex(tianyi, Eigen::Vector4f(-0.5, -0.5, 0.0, 1.0), Eigen::Vector2f(0.0, 0.0)),
//		Vertex(tianyi, Eigen::Vector4f(0.5, -0.5, 0.0, 1.0), Eigen::Vector2f(0.0, 1.0)),
//		Vertex(tianyi, Eigen::Vector4f(0.0, 0.5, 0.0, 1.0), Eigen::Vector2f(1.0, 0.5))
//	};
//	std::vector<Vertex> vertices_2 = {
//		Vertex(aling, Eigen::Vector4f(1, 0, 0, 1), Eigen::Vector2f(1, 0)),
//		Vertex(aling, Eigen::Vector4f(0, 1, 0, 1), Eigen::Vector2f(0, 1)),
//		Vertex(aling, Eigen::Vector4f(1, 1, 0, 1), Eigen::Vector2f(1, 1))
//	};
/*
	float vertices[] = {
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			// front
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

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
			Eigen::Vector4f position(data[i], data[i+1], data[i+2], 1.0);
			Eigen::Vector2f texCoords(data[i+3], data[i+4]);
			result.emplace_back(color, position, texCoords);
		}
		return result;
	};
	std::vector<Eigen::Vector3f> positions = {
			Eigen::Vector3f( 0.0,  0.0,  0.0),
			Eigen::Vector3f( 2.0,  5.0, -15.0),
			Eigen::Vector3f(-1.5, -2.2, -2.5),
			Eigen::Vector3f(-3.8, -2.0, -12.3),
			Eigen::Vector3f( 2.4, -0.4, -3.5),
			Eigen::Vector3f(-1.7,  3.0, -7.5),
			Eigen::Vector3f( 1.3, -2.0, -2.5),
			Eigen::Vector3f( 1.5,  2.0, -2.5),
			Eigen::Vector3f( 1.5,  0.2, -1.5),
			Eigen::Vector3f(-1.3,  1.0, -1.5)
	};
	std::vector<Vertex> m_vertices = toVertices(vertices, sizeof(vertices) / sizeof(float), Eigen::Vector4f(0.5, 0.5, 0.5, 1.0));
	std::string projectRoot = getProjectPath();
	std::string vertexShaderPath = projectRoot + "/Shader/Triangle.vert";
	std::string fragmentShaderPath = projectRoot + "/Shader/Triangle.frag";
	std::string texturePath = "/Texture/wall.jpg";
	Camera camera({0, 0, 3.0f}, {0, 0, 0.0f}, {0, 1, 0},
		   45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	for(int k = 0; k < positions.size(); k++){
//	for(int k = 0; k < 2; k++){
			auto tgr = std::make_unique<TriangleRender>(m_vertices, vertexShaderPath, fragmentShaderPath, texturePath);
			tgr->setPosition(Eigen::Vector4f(positions[k][0], positions[k][1], positions[k][2], 1.0));
			tgr->setCamera(camera);
			MainRenderThread::add(std::move(tgr));
//		}
	}
 */
//	auto tmpT = std::make_unique<TriangleRender>(m_vertices, vertexShaderPath, fragmentShaderPath, texturePath);
//	MainRenderThread::add(std::move(tmpT));
//	auto tgr1 = std::make_unique<TriangleRender>(m_vertices, vertexShaderPath, fragmentShaderPath, texturePath);
//	MainRenderThread::add(std::move(tgr1));
//	auto tgr2 = std::make_unique<TriangleRender>(vertices_2, vertexShaderPath, fragmentShaderPath, texturePath);
//	MainRenderThread::add(std::move(tgr2));
//	Camera camera({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
	Camera camera({0, 0, 3.0f}, {0.0f, 0.0f, 0.0f}, {0, 1, 0},
		   45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	std::string projectRoot = getProjectPath();
	//std::string pointVertexShaderPath = projectRoot + "/Shader/Points/Point.vert";
	std::string pointVertexShaderPath = "D:/Code/LearnOpenGL/Shader/Points/Point.vert";
	//std::string pointFragmentShaderPath = projectRoot + "/Shader/Points/Point.frag";
	std::string pointFragmentShaderPath = "D:/Code/LearnOpenGL/Shader/Points/Point.frag";
//	auto position = Eigen::Vector4f(15, 10, 30, 10);
	auto position = Eigen::Vector4f(-1, -1, 0, 1);
	std::string texturePath = "/Texture/wall.jpg";
//	auto ptr = std::make_unique<PointRender>(500, pointVertexShaderPath, pointFragmentShaderPath);
//	ptr->setPosition(position);
//	ptr->setCamera(camera);
//	MainRenderThread::add(std::move(ptr));
//	auto sim = SimulatorCache(50000);
//	std::vector<Vertex> vertices;
//	auto particles = sim.getParticles();
//	for (auto particle : particles){
//		vertices.push_back(Vertex::particlesToVertex(particle));
//		vertices.back().color = {1.0f, 1.0f, 1.0f, 1.0f};
//		vertices.back().position = {vertices.back().position.x(), vertices.back().position.y(), vertices.back().position.z(), 20.0f};
//		LOG_INFO << "PointRender::update()\n" << vertices.back().position << '\n';
//	}
//	auto tgr = std::make_unique<TriangleRender>(vertices, pointVertexShaderPath, pointFragmentShaderPath, texturePath);
//	tgr->setPosition({0, 0, 0, 1});
//	tgr->setCamera(camera);
//	MainRenderThread::add(std::move(tgr));
	auto ptr = std::make_unique<PointRender>(25000, pointVertexShaderPath, pointFragmentShaderPath);
	ptr->setPosition(position);
	ptr->setCamera(camera);
//	ptr->setVertices(vertices);
	MainRenderThread::add(std::move(ptr));
	MainRenderThread::instance().waitForExit();
	return 0;
}