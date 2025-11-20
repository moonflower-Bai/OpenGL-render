//
// Created by Jingren Bai on 25-9-9.
//

#ifndef LEARNOPENGL_POINTRENDER_H
#define LEARNOPENGL_POINTRENDER_H

#include "Src/Rendering/old_primitive_pipeline/Base/ObjectRender.h"
#include "Src/Rendering/Assets/fluid/CPU_process/FluidSimulator.h"
#include "Src/Shader/Shader.h"

class PointRender : public ObjectRender {
private:
	Simulator simulator;
	Shader m_shader;
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
public:
	PointRender(int particleNums, std::string vertexShaderPath, std::string fragmentShaderPath);
	void setVertices(std::vector<Vertex> vertices);
	std::vector<Vertex> getVertices();

	void init() override;
	void render() override;
	void update() override;
	void destroy() override;
};


#endif //LEARNOPENGL_POINTRENDER_H
