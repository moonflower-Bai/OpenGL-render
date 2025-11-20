//
// Created by Jingren Bai on 25-11-12.
//

#include <random>
#include "GPU_FluidSimulator.h"
#include "utils/getProgramPath.h"

GLuint GPU_FluidSimulator::createComputeShaderProgram(const std::string& file)
{
	std::string path = getProgramPath() + "/shaders/" + file;
	// 1. 读取文件
	ReadShader rs(path.c_str());
	const char* src = rs.getShader();

	// 2. 创建 shader 对象
	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	// 3. 检查编译错误
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		LOG_ERROR << "Compute Shader compilation failed:\n" << infoLog;
	}

	// 4. 创建程序对象并链接
	GLuint program = glCreateProgram();
	glAttachShader(program, shader);
	glLinkProgram(program);

	// 5. 检查链接错误
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		LOG_ERROR << "Compute Shader program linking failed:\n" << infoLog;
	}

	// 6. 删除中间 shader 对象
	glDeleteShader(shader);

	return program;
}

GPU_FluidSimulator::GPU_FluidSimulator(int numParticles)
	: clearGridProgram(0),
	  predictAndBuildGridProgram(0),
	  computeLambdaProgram(0),
	  computeDeltaProgram(0),
	  epilogueProgram(0),
	  particleSSBO(0),
	  cellIndexSSBO(0),
	  cellCountSSBO(0),
	  paramsUBO(0)
{
	params.numParticles = numParticles;
}
GPU_FluidSimulator::~GPU_FluidSimulator() {
	// 只有在 OpenGL 已初始化且 id 非 0 时才删除
	if (clearGridProgram)        glDeleteProgram(clearGridProgram);
	if (predictAndBuildGridProgram) glDeleteProgram(predictAndBuildGridProgram);
	if (computeLambdaProgram)    glDeleteProgram(computeLambdaProgram);
	if (computeDeltaProgram)     glDeleteProgram(computeDeltaProgram);
	if (epilogueProgram)         glDeleteProgram(epilogueProgram);

	if (particleSSBO)            glDeleteBuffers(1, &particleSSBO);
	if (cellIndexSSBO)           glDeleteBuffers(1, &cellIndexSSBO);
	if (cellCountSSBO)           glDeleteBuffers(1, &cellCountSSBO);
	if (paramsUBO)               glDeleteBuffers(1, &paramsUBO);

}

void GPU_FluidSimulator::onAttach() {
	LOG_INFO << "GPU_FluidSimulator attached";
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-0.25f, 0.25f); // 创建一个随机数生成器, 范围在-0.25到0.25之间
	LOG_INFO << "FluidSimulator init";
	Eigen::Vector3f initPos = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
//	LOG_INFO << "boundary = " << params.boundaryMin << ", " << params.boundaryMax << ", grid = " << params.gridSize << ", cellSize = " << params.cellSize << ". ";
	float CubeSize = std::ceil(pow(params.numParticles, 1.0f / 3.0f));
	float spacing = 1.0f;
	int numPerRow = (int)(CubeSize / spacing) + 1;
	int numPerFloor = numPerRow * numPerRow;
	for (int i = 0; i < params.numParticles; i++) {
		int floor = i / numPerFloor;
		int row = (i % numPerFloor) / numPerRow;
		int col = (i % numPerFloor) % numPerRow;
		Eigen::Vector3f pos = Eigen::Vector3f(static_cast<float>(col) * spacing + dis(gen),
											  static_cast<float>(floor) * spacing + dis(gen),
											  static_cast<float>(row) * spacing + dis(gen)) + initPos;
//		particlePos[i] = Eigen::Vector4f(pos.x(), pos.y(), pos.z(), 1.0f);
		particlePos.emplace_back(pos);
//		LOG_INFO << "particle " << i << " pos: " << particlePos[i].pos.transpose();
	}
//	LOG_INFO << "row: " << numPerRow << ", col: " << numPerRow << ", floor: " << numPerFloor;
//	LOG_INFO << "Init particle nums is " << params.numParticles << ". " << "Finished init.";
}
void GPU_FluidSimulator::onStart() {
	// 初始化 SSBO 和 UBO
	LOG_INFO << "particlePos.size() = " << particlePos.size();
	LOG_INFO << "Expected = " << params.numParticles;
	LOG_INFO << "sizeof(GPU_Particle) = " << sizeof(GPU_Particle);
	glGenBuffers(1, &particleSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, params.numParticles * sizeof(GPU_Particle), particlePos.data(), GL_DYNAMIC_DRAW); // pos + vel
	// bind to shader binding 1 (Particles uses binding = 1 in fluidCommon.glsl)
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particleSSBO);

	glGenBuffers(1, &cellIndexSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, cellIndexSSBO);
//	GLuint totalCells = params.gridSize.x() * params.gridSize.y() * params.gridSize.z();
	GLuint totalCells = params.gridSizeX * params.gridSizeY * params.gridSizeZ;
	glBufferData(GL_SHADER_STORAGE_BUFFER, totalCells * params.maxNeighboursPerCell * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	// bind to shader binding 2 (CellParticleIndices uses binding = 2)
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, cellIndexSSBO);

	glGenBuffers(1, &cellCountSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, cellCountSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, totalCells * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	// bind to shader binding 3 (CellCounts uses binding = 3)
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, cellCountSSBO);

	glGenBuffers(1, &paramsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, paramsUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(GPUFluidParams), &params, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, paramsUBO);

	// 创建 compute shader 程序
	std::string projectRoot = getProjectPath();
	clearGridProgram = createComputeShaderProgram("csClearGrid.comp");
	predictAndBuildGridProgram = createComputeShaderProgram("csPredictAndBuildGrid.comp");
	computeLambdaProgram = createComputeShaderProgram("csComputeLambda.comp");
	computeDeltaProgram = createComputeShaderProgram("csComputeDeltaAndApply.comp");
	epilogueProgram = createComputeShaderProgram("csEpilogue.comp");
}

void GPU_FluidSimulator::uploadParams() {
	glBindBuffer(GL_UNIFORM_BUFFER, paramsUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GPUFluidParams), &params);
}
void GPU_FluidSimulator::dispatchComputeShader(GLuint program, GLuint numGroups) {
	if (!glDispatchCompute) {
		LOG_ERROR << "glDispatchCompute is NULL — OpenGL context missing in this thread!";
		return;
	}
	if (!glIsProgram(program)) {
		LOG_ERROR << "Invalid compute program — skipping dispatch.";
		return;
	}
	glUseProgram(program);
	glDispatchCompute(numGroups, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
}

void GPU_FluidSimulator::Update(float deltaTime) {
	uploadParams();

	// 1. 计算粒子和网格两个不同的 group 数
	GLuint groupsParticles = (params.numParticles + 255) / 256;

	GLuint totalCells = params.gridSizeX * params.gridSizeY * params.gridSizeZ;
	GLuint groupsCells = (totalCells + 255) / 256;

	dispatchComputeShader(clearGridProgram, groupsCells);

	dispatchComputeShader(predictAndBuildGridProgram, groupsParticles);

	for (int iter = 0; iter < params.pbfNumIters; ++iter) {
		dispatchComputeShader(computeLambdaProgram, groupsParticles);
		dispatchComputeShader(computeDeltaProgram, groupsParticles);
	}

	dispatchComputeShader(epilogueProgram, groupsParticles);
}

void GPU_FluidSimulator::onDetach() {
	LOG_INFO << "GPU_FluidSimulator detached";
}

GLuint GPU_FluidSimulator::getParticleSSBO() const {
	return particleSSBO;
}

GLuint GPU_FluidSimulator::getCellIndexSSBO() const {
	return cellIndexSSBO;
}

GLuint GPU_FluidSimulator::getCellCountSSBO() const {
	return cellCountSSBO;
}

GLuint GPU_FluidSimulator::getParamsUBO() const {
	return paramsUBO;
}

const GPUFluidParams &GPU_FluidSimulator::getParams() const {
	return params;
}
