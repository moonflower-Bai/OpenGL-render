//
// Created by Jingren Bai on 25-7-15.
//

#ifndef LEARNOPENGL_TRIANGLERENDER_H
#define LEARNOPENGL_TRIANGLERENDER_H

#include "Rendering/old_primitive_pipeline/Base/ObjectRender.h"
#include "Shader/Shader.h"

class TriangleRender : public ObjectRender {
private:
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
	Texture m_texture;
	Shader m_shader;
public:
	explicit TriangleRender(const std::vector<Vertex> vertices, std::string vertexShaderPath, std::string fragmentShaderPath, std::string texturePath);
	TriangleRender();
	void setVertices(std::vector<Vertex> vertices);
	std::vector<Vertex> getVertices();
	void setVertexShaderPath(std::string vertexShaderPath);
	void setFragmentShaderPath(std::string fragmentShaderPath);
	void setTexturePath(std::string texturePath);
	void init() override;
	void update() override { /* empty */}
	void render() override;
	void destroy() override;

	Shader* getShader();
};


#endif //LEARNOPENGL_TRIANGLERENDER_H
