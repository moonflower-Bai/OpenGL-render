//
// Created by Jingren Bai on 25-11-12.
//

#include "GPU_FluidRender.h"
#include "GPU_FluidSimulator.h"
#include "utils/log.cpp"
#include "Shader/Shader.h"
#include "utils/getProgramPath.h"
#include <fstream>
#include <sstream>
#include "Rendering/Primitives/Entity/Entity.h"
#include "Rendering/Pipeline/RenderThread_ECS.h"

GLuint GPU_FluidRender::createShaderProgram(const std::string& vertPath, const std::string& fragPath) {
	std::string projectRoot = getProjectPath();
	std::string vertexFullPath = projectRoot + "/" + vertPath;
	std::string fragmentFullPath = projectRoot + "/" + fragPath;
	m_shader = Shader(vertexFullPath, fragmentFullPath);
	// Transfer ownership of the created program to caller to avoid deletion in Shader::~Shader()
	GLuint program = m_shader.getProgramID();
	return program;
}

// Implement destructor to satisfy the linker. Perform GL cleanup only when GL context is ready.
GPU_FluidRender::~GPU_FluidRender() {
	if (RenderThread_ECS::isGLReady()) {
		if (m_renderProgram) {
			glDeleteProgram(m_renderProgram);
			m_renderProgram = 0;
		}
		if (m_vao) {
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
		if (m_vbo) {
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}
	} else {
		// If GL isn't ready, assume onDetach() or the render thread will handle cleanup when appropriate.
	}
}

void GPU_FluidRender::onAttach() {
	LOG_INFO << "[GPU_FluidRender] attached.";
}

void GPU_FluidRender::onStart() {
	// Called once when GL context is ready (Component::tick guarantees isGLReady())
	if (!ensureInitialized()) {
		LOG_ERROR << "[GPU_FluidRender] Failed to initialize GL resources in onStart().";
	}
}

bool GPU_FluidRender::ensureInitialized() {
	if (m_renderProgram && m_vao) return true; // already initialized

	if (!RenderThread_ECS::isGLReady()) {
		LOG_INFO << "[GPU_FluidRender] GL not ready yet, deferring initialization.";
		return false;
	}

	auto entity = getEntity();
	if (!entity) {
		LOG_ERROR << "[GPU_FluidRender] No owning entity, cannot init.";
		return false;
	}
	auto sim = entity->getComponent<GPU_FluidSimulator>();
	if (!sim) {
		LOG_ERROR << "[GPU_FluidRender] Missing GPU_FluidSimulator component.";
		return false;
	}

	GLuint particleSSBO = sim->getParticleSSBO();
	m_numParticles = sim->getParams().numParticles;
	if (particleSSBO == 0 || m_numParticles <= 0) {
		LOG_INFO << "[GPU_FluidRender] SSBO not ready or no particles yet (ssbo=" << particleSSBO << ", num=" << m_numParticles << ")";
		return false;
	}

	// Create shader program
	m_renderProgram = createShaderProgram(
		"Rendering/Assets/fluid/GPU_process/shaders/fluid_render.vert",
		"Rendering/Assets/fluid/GPU_process/shaders/fluid_render.frag"
	);
	if (!m_renderProgram) {
		LOG_ERROR << "[GPU_FluidRender] Failed to create render shader program.";
		return false;
	}

	// Setup VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Bind the SSBO as array buffer to use as vertex source
	glBindBuffer(GL_ARRAY_BUFFER, particleSSBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPU_Particle), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LOG_INFO << "[GPU_FluidRender] Initialized GL resources: program=" << m_renderProgram << ", vao=" << m_vao << ", particles=" << m_numParticles;
	return true;
}

void GPU_FluidRender::Update(float deltaTime) {
	// Ensure initialization; if not ready, skip this frame
	if (!ensureInitialized()) {
		return;
	}
	m_shader.use();
//	m_shader.setMat4("model", getEntity()->getTransform().getModelMatrix());

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		LOG_ERROR << "GL error after useProgram: " << err;

		// Try to get program info log
		GLint infoLogLen = 0;
		glGetProgramiv(m_renderProgram, GL_INFO_LOG_LENGTH, &infoLogLen);
		if (infoLogLen > 0) {
			std::string info(infoLogLen, '\0');
			glGetProgramInfoLog(m_renderProgram, infoLogLen, nullptr, info.data());
			LOG_ERROR << "Program info log:\n" << info;
		}

		// Validate program to get validation status
		glValidateProgram(m_renderProgram);
		GLint validateStatus = 0;
		glGetProgramiv(m_renderProgram, GL_VALIDATE_STATUS, &validateStatus);
		LOG_ERROR << "Program validate status: " << validateStatus;

		return; // don't proceed to draw
	}

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_numParticles);
	glBindVertexArray(0);
}

void GPU_FluidRender::onDetach() {
	if (m_renderProgram) glDeleteProgram(m_renderProgram);
	if (m_vao) glDeleteVertexArrays(1, &m_vao);
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
}
