//
// Created by Jingren Bai on 25-7-15.
//
#define _CRT_SECURE_NO_WARNINGS

#include "ReadShader.h"
#include "utils/log.cpp"
ReadShader::ReadShader(const char *path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		LOG_ERROR << "Can't open file " << path;
		return;
	}
	char line[1024];
	while (file.getline(line, 1024)) {
		shader += line;
		shader += "\n";
	}
	shader += '\0';
}
ReadShader::ReadShader(std::string path) {
//	ReadShader(path.c_str());
	std::ifstream file(path);
	if (!file.is_open()) {
		LOG_ERROR << "Can't open file at path: " << path;
		return;
	}
	char line[1024];
	while (file.getline(line, 1024)) {
		shader += line;
		shader += "\n";
	}
	shader += '\0';
}

char *ReadShader::getShader() const {
	char *shader_c = new char[shader.size()+1];
	strcpy(shader_c, shader.c_str());
	return shader_c;
//	return shader.c_str();
}