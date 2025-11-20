//
// Created by Jingren Bai on 25-10-26.
//

#ifndef LEARNOPENGL_BALLRENDER_H
#define LEARNOPENGL_BALLRENDER_H

#include "Src/Rendering/old_primitive_pipeline/Base/ObjectRender.h"
#include "Src/Shader/Shader.h"
#include "Rendering/Core/Texture.h"
#include <cmath>

class BallRender : public ObjectRender {
private:
	std::vector<unsigned int> indices;
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
	Shader m_shader;
	float radius = 1.0f;
public:
	BallRender(float radius, std::string vertexShaderPath, std::string fragmentShaderPath);

	void init() override;
	void update() override {}
	void render() override;

	void setShader(const Shader& _shader){
		m_shader = _shader;
	}

	[[nodiscard]] Shader* getShader() {
		return &m_shader;
	}
};


#endif //LEARNOPENGL_BALLRENDER_H
