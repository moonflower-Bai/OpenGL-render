//
// Created by Jingren Bai on 25-10-26.
//

#ifndef LEARNOPENGL_BALLRENDER_H
#define LEARNOPENGL_BALLRENDER_H

#include "Rendering/Primitives/Base/ObjectRender.h"
#include "Shader/Shader.h"
#include "Rendering/Core/Texture.h"

class BallRender : public ObjectRender {
private:
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	Shader shader;
	Texture Texture;
public:
	BallRender();
	void init() override;
	void update() override;
	void render() override;
};


#endif //LEARNOPENGL_BALLRENDER_H
