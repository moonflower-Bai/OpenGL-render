//
// Created by Jingren Bai on 25-8-6.
//

#include "Shader.h"

void Shader::checkCompileErrors(unsigned int shader){
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		LOG_ERROR << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << '\n';
		return;
	}
}

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
	ReadShader VertexShader(vertexPath);
	char* vertexShader = VertexShader.getShader();
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID);

	ReadShader FragmentShader(fragmentPath);
	char* fragmentShader = FragmentShader.getShader();
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShader, nullptr);
	glCompileShader(fragmentShaderID);
	checkCompileErrors(fragmentShaderID);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	LOG_INFO << "Get Shader Program ID: " << programID << '\n';

	int success = 0;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		LOG_ERROR << "ERROR::SHADER::PROGRAM::LINKING\n" << infoLog << '\n';
		exit(1);
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}
void Shader::use(){
	glUseProgram(programID);
}
void Shader::setInt(const std::string &name, int value){
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value){
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
void Shader::setVec3(const std::string &name, float x, float y, float z){
	glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}
void Shader::setVec3(const std::string &name, Eigen::Vector3f value){
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, value.data());
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w){
	glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}
void Shader::setVec4(const std::string &name, Eigen::Vector4f value){
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, value.data());
}

Shader::~Shader() {
	glDeleteProgram(programID);
}
std::string Shader::getVertexPath() {
	return vertexPath;
}
std::string Shader::getFragmentPath() {
	return fragmentPath;
}
unsigned int Shader::getProgramID() {
	return programID;
}
Shader::Shader(const Shader &_shader) {
	this->vertexPath = _shader.vertexPath;
	this->fragmentPath = _shader.fragmentPath;
	ReadShader VertexShader(vertexPath);
	char* vertexShader = VertexShader.getShader();
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID);

	ReadShader FragmentShader(fragmentPath);
	char* fragmentShader = FragmentShader.getShader();
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShader, nullptr);
	glCompileShader(fragmentShaderID);
	checkCompileErrors(fragmentShaderID);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	LOG_INFO << "Get Shader Program ID: " << programID << '\n';

	int success = 0;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		LOG_ERROR << "ERROR::SHADER::PROGRAM::LINKING\n" << infoLog << '\n';
		exit(1);
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}
Shader::Shader(Shader &&_shader) noexcept{
	programID = _shader.programID;
	_shader.programID = 0;
}
Shader& Shader::operator=(Shader _shader) {
	std::swap(programID, _shader.programID);
	return *this;
}

void Shader::setMat4(const std::string &name, Eigen::Matrix4f value) {
	Eigen::Matrix<float, 4, 4, Eigen::RowMajor> value_ = value.transpose();
//	value_.transposeInPlace();
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value_.data());
}

void Shader::setMat4(const std::string &name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
