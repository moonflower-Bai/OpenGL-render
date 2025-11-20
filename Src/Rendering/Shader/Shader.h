//
// Created by Jingren Bai on 25-8-6.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <string>
#include "cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/glad/glad.h"
#include "cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/GLFW/glfw3.h"
#include "cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/Eigen/Eigen"
#include "utils/ReadShader.h"

class Shader {
private:
	unsigned int programID;
	void checkCompileErrors(unsigned int shader);
	std::string vertexPath;
	std::string fragmentPath;
public:
	Shader() = default;
	Shader(const Shader &);
	Shader(Shader &&) noexcept;
	Shader(std::string vertexPath, std::string fragmentPath);
	Shader& operator=(Shader);
	~Shader();
	void use();
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void setVec3(const std::string &name, float x, float y, float z);
	void setVec3(const std::string &name, Eigen::Vector3f value);
	void setVec4(const std::string &name, float x, float y, float z, float w);
	void setVec4(const std::string &name, Eigen::Vector4f value);
	void setMat4(const std::string &name, Eigen::Matrix4f value);
	void setMat4(const std::string &name, glm::mat4 value);
	std::string getVertexPath();
	std::string getFragmentPath();
	unsigned int getProgramID();
	// Transfer ownership of the internal program id (sets internal id to 0)
	unsigned int takeProgramID();
};


#endif //LEARNOPENGL_SHADER_H
