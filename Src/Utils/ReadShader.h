//
// Created by Jingren Bai on 25-7-15.
//

#ifndef LEARNOPENGL_READSHADER_H
#define LEARNOPENGL_READSHADER_H

#include "log.cpp"
#include <fstream>
#include <map>

class ReadShader {
private:

	std::string processShader(const std::string& path);
	std::string shaderSource;
	static std::map<std::string, std::string> includeCache; // 全局缓存已加载的文件
public:
	ReadShader(const std::string &path);
	const char *getShader() const;

	static std::string readFile(const std::string &path);
};


#endif //LEARNOPENGL_READSHADER_H
