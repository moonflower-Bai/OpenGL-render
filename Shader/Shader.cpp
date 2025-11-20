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
    // Defensive check: ensure GL function pointers are loaded
    if (!glCreateShader) {
        LOG_ERROR << "OpenGL functions not initialized (glCreateShader is NULL). Ensure gladLoadGLLoader was called.";
        programID = 0;
        return;
    }

	ReadShader VertexShader(vertexPath);
	const char* vertexShader = VertexShader.getShader();
	LOG_INFO << "Get Vertex Shader Source: " << vertexShader << '\n';
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if(!vertexShaderID){
		LOG_ERROR << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexShaderID << '\n';
	}
//	LOG_INFO << "Get Vertex Shader ID: " << vertexShaderID << '\n';
	glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID);

	ReadShader FragmentShader(fragmentPath);
	const char* fragmentShader = FragmentShader.getShader();
	LOG_INFO << "Get Fragment Shader Source: " << fragmentShader << '\n';
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if(!fragmentShaderID){
		LOG_ERROR << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentShaderID << '\n';
	}
	glShaderSource(fragmentShaderID, 1, &fragmentShader, nullptr);
	glCompileShader(fragmentShaderID);
	checkCompileErrors(fragmentShaderID);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Log attached shaders and link status
	GLint attachedCount = 0;
	glGetProgramiv(programID, GL_ATTACHED_SHADERS, &attachedCount);
	LOG_INFO << "Attached shaders: " << attachedCount;

	LOG_INFO << "Get Shader Program ID: " << programID << '\n';

	int success = 0;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		LOG_ERROR << "ERROR::SHADER::PROGRAM::LINKING\n" << infoLog << '\n';
		exit(1);
	}

	// clear GL errors that might have been set
	while (glGetError() != GL_NO_ERROR) {}
	// Validate program and dump active attributes/uniforms
	glValidateProgram(programID);
	GLint validateStatus = 0;
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &validateStatus);
	LOG_INFO << "Program validate status: " << validateStatus;

	GLint activeAttribs = 0;
	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &activeAttribs);
	LOG_INFO << "Active attributes: " << activeAttribs;
	for (int i = 0; i < activeAttribs; ++i) {
		char name[256];
		GLsizei len = 0; GLint size = 0; GLenum type = 0;
		glGetActiveAttrib(programID, i, sizeof(name), &len, &size, &type, name);
		LOG_INFO << "Attrib[" << i << "] name='" << name << "' size=" << size << " type=" << type;
	}

	GLint activeUniforms = 0;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &activeUniforms);
	LOG_INFO << "Active uniforms: " << activeUniforms;
	for (int i = 0; i < activeUniforms; ++i) {
		char name[256];
		GLsizei len = 0; GLint size = 0; GLenum type = 0;
		glGetActiveUniform(programID, i, sizeof(name), &len, &size, &type, name);
		LOG_INFO << "Uniform[" << i << "] name='" << name << "' size=" << size << " type=" << type;
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
    if (programID != 0) {
        glDeleteProgram(programID);
        programID = 0;
    }
}

unsigned int Shader::getProgramID() {
    return programID;
}

unsigned int Shader::takeProgramID() {
    unsigned int id = programID;
    programID = 0;
    return id;
}

Shader::Shader(const Shader &_shader) {
	this->vertexPath = _shader.vertexPath;
	this->fragmentPath = _shader.fragmentPath;
	ReadShader VertexShader(vertexPath);
	const char* vertexShader = VertexShader.getShader();
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID);

	ReadShader FragmentShader(fragmentPath);
	const char* fragmentShader = FragmentShader.getShader();
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
