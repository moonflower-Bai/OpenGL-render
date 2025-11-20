//
// Created by Jingren Bai on 25-11-12.
//

#ifndef LEARNOPENGL_GPU_FLUIDSIMULATOR_H
#define LEARNOPENGL_GPU_FLUIDSIMULATOR_H
// C++ Headers
#include <vector>
// External Headers
#include <Eigen/Eigen>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Internal Headers
#include "utils/ReadShader.h"
#include "ECS/Components/Component.h"
#include "GPU_Particle.h"

//struct GPUFluidParams {
//	float dt = 0.05f;
//	float h = 1.0f;
//	float mass = 1.0f;
//	float rho = 1000.0f;
//	float neighbourRadius = 1.0f;
//	float lambdaEpsilon = 0.001f;
//	Eigen::Vector3i gridSize = {32, 32, 32};
//	float cellSize = 1.0f;
//	Eigen::Vector3f boundaryMin = {0.0f, 0.0f, 0.0f};
//	Eigen::Vector3f boundaryMax = {32.0f, 32.0f, 32.0f};
//	int maxNeighboursPerCell = 64;
//	int numParticles = 0;
//	int pbfNumIters = 4;
//};

struct GPUFluidParams {
	// --- 第一块：6 个 float ---
	float dt = 0.0016f;
	float h = 1.1f;
	float mass = 1.0f;
	float rho = 1.0f;
	float neighbourRadius = 1.01f;
	float lambdaEpsilon = 100.0f;
	// std140 这里为了让下一个 ivec3 按 16 对齐，会留 2 个 float 空位
	float _padScalar0 = 0.0f;
	float _padScalar1 = 0.0f;

	// --- gridSize + cellSize ---
	int   gridSizeX = 32;
	int   gridSizeY = 32;
	int   gridSizeZ = 32;
	float cellSize = 1.1f;

	// --- boundaryMin + pad ---
	float boundaryMinX = 0.0f;
	float boundaryMinY = 0.0f;
	float boundaryMinZ = 0.0f;
	float _pad0 = 0.0f;  // 对应 GLSL 里的 _pad0

	// --- boundaryMax + pad ---
	float boundaryMaxX = 32.0f;
	float boundaryMaxY = 100.0f;
	float boundaryMaxZ = 32.0f;
	float _pad1 = 0.0f;  // 对应 GLSL 里的 _pad1

	// --- 后面 3 个 int + pad ---
	int maxNeighboursPerCell = 40;
	int numParticles;
	int pbfNumIters = 4;
	int _pad2 = 0; // 对齐到 16 的倍数
};


class GPU_FluidSimulator : public Component{
private: // 参数
	GPUFluidParams params;
//	Eigen::Vector4f particlePos[30001]; // 存储粒子位置的数组，最多30000个粒子
	std::vector<GPU_Particle> particlePos;
public:
	const GPUFluidParams &getParams() const;

private: // 程序对象
	GLuint clearGridProgram;
	GLuint predictAndBuildGridProgram;
	GLuint computeLambdaProgram;
	GLuint computeDeltaProgram;
	GLuint epilogueProgram;
public:
	GLuint getParticleSSBO() const;
	GLuint getCellIndexSSBO() const;
	GLuint getCellCountSSBO() const;
	GLuint getParamsUBO() const;

private:// 缓冲对象
	GLuint particleSSBO; // 粒子位置 SSBO
	GLuint cellIndexSSBO; // 网格索引 SSBO
	GLuint cellCountSSBO; // 网格计数 SSBO
	GLuint paramsUBO; // 参数 UBO
private: // 变量
	GLuint createComputeShaderProgram(const std::string& path);
public:
	explicit GPU_FluidSimulator(int numParticles);
	~GPU_FluidSimulator() override;

	void onAttach() override;
	void onStart() override;
	void onDetach() override;
	void Update(float deltaTime) override;

	void uploadParams();
	void dispatchComputeShader(GLuint program, GLuint numGroups);
};


#endif //LEARNOPENGL_GPU_FLUIDSIMULATOR_H
