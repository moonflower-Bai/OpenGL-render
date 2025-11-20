//
// Created by Jingren Bai on 25-11-12.
//

#ifndef LEARNOPENGL_GPU_FLUIDRENDER_H
#define LEARNOPENGL_GPU_FLUIDRENDER_H

#include <string>
#include <memory>

#include <glad/glad.h>

#include "ECS/Components/Component.h"
#include "Shader/Shader.h"

class GPU_FluidRender : public Component{
private:
	GLuint m_renderProgram = 0;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	int m_numParticles = 0;

	Shader m_shader;

public:
	GPU_FluidRender() = default;
	~GPU_FluidRender() override;

	void onAttach() override;
	void onDetach() override;
	void onStart() override;
	void Update(float deltaTime) override;

private:
	GLuint createShaderProgram(const std::string& vertPath, const std::string& fragPath);
	// Ensure GL resources (shader/VAO) are created. Returns true if initialized.
	bool ensureInitialized();
};


#endif //LEARNOPENGL_GPU_FLUIDRENDER_H
