//
// Created by Jingren Bai on 25-8-26.
//

#include "RenderBuilder.h"

RenderBuilder &RenderBuilder::withVertices(std::vector<Vertex> vertices) {
	this->vertices = std::move(vertices);
	return *this;
}

RenderBuilder &RenderBuilder::withIndices(std::vector<unsigned int> indices) {
	this->indices = std::move(indices);
	return *this;
}

RenderBuilder& RenderBuilder::withShader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	shader = ResourceManager::getShader(vertexShaderPath, fragmentShaderPath);
	return *this;
}

RenderBuilder &RenderBuilder::withDefaultShader() {
	shader = ResourceManager::getDefaultShader();
	return *this;
}

RenderBuilder &RenderBuilder::withTexture(std::string texturePath) {
	enableTexture = true;
	texture = ResourceManager::getTexture(texturePath);
	return *this;
}

RenderBuilder &RenderBuilder::withoutTexture() {
	enableTexture = false;
//	texture = nullptr;
	return *this;
}

RenderBuilder &RenderBuilder::withDepthTesting(bool enable) {
	enableDepthTesting = enable;
	return *this;
}

std::unique_ptr<ObjectRender> RenderBuilder::build() {
	return std::unique_ptr<ObjectRender>();
}

std::unique_ptr<TriangleRendering> RenderBuilder::buildTriangle() {
	auto triangle = std::make_unique<TriangleRendering>();
//	triangle->setVertices(vertices);
//	triangle->setShader(shader);
//	if(enableTexture && !texture.empty()) {
//		triangle->setTexture(texture);
//	}
//
//	triangle->setDepthTesting(enableDepthTesting);
	return triangle;
}

RenderBuilder RenderBuilder::createDefault() {
	//FIXME: 编译器会报错，暂时用这个占位
	return RenderBuilder()
	.withDefaultShader()
	.withDepthTesting(true);
//	return RenderBuilder();
}

RenderBuilder::RenderBuilder() {
	enableTexture = false;
}

RenderBuilder::RenderBuilder(const RenderBuilder &builder) {
	
}

