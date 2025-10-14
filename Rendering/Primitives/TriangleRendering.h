//
// Created by Jingren Bai on 25-7-15.
//

#ifndef LEARNOPENGL_TRIANGLERENDERING_H
#define LEARNOPENGL_TRIANGLERENDERING_H

#include "ObjectRender.h"
#include "Shader/Shader.h"

class TriangleRendering : public ObjectRender {
private:
//	Eigen::Matrix<float, 3, 3, Eigen::RowMajor> vertices;
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
	Texture m_texture;
	Shader m_shader;
public:
	explicit TriangleRendering(const std::vector<Vertex> vertices, std::string vertexShaderPath, std::string fragmentShaderPath, std::string texturePath);
	TriangleRendering();
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


#endif //LEARNOPENGL_TRIANGLERENDERING_H
