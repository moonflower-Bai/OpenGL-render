//
// Created by Jingren Bai on 25-8-26.
//

#ifndef LEARNOPENGL_RENDERBUILDER_H
#define LEARNOPENGL_RENDERBUILDER_H

#include <vector>
#include <string>
#include <Eigen/Eigen>
#include "Rendering/Core/Vertex.h"
#include "ObjectRender.h"
#include "TriangleRendering.h"
#include "Rendering/Core/ResourceManager.h"

class RenderBuilder {
private:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Shader shader;
	Texture texture;
	Eigen::Vector4i color;
	bool enableTexture = false;
	bool enableLight = false;
	bool enableDepthTesting = true;

public:
	explicit RenderBuilder();
//	RenderBuilder() = default;
	RenderBuilder& withVertices(std::vector<Vertex> vertices);
	RenderBuilder& withIndices(std::vector<unsigned int> indices);

	RenderBuilder& withShader(std::string vertexShaderPath, std::string fragmentShaderPath);
	RenderBuilder& withDefaultShader();

	RenderBuilder& withTexture(std::string texturePath);
	RenderBuilder& withoutTexture();

	RenderBuilder& withDepthTesting(bool enable);

	std::unique_ptr<ObjectRender> build();
	std::unique_ptr<TriangleRendering> buildTriangle();

	static RenderBuilder createDefault();
};


#endif //LEARNOPENGL_RENDERBUILDER_H
