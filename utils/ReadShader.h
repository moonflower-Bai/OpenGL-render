//
// Created by Jingren Bai on 25-7-15.
//

#ifndef LEARNOPENGL_READSHADER_H
#define LEARNOPENGL_READSHADER_H

#include "utils/log.cpp"
#include <fstream>

class ReadShader {
private:
	std::string shader;
public:
	ReadShader(const char* path);
	ReadShader(std::string path);
	char *getShader() const;
};


#endif //LEARNOPENGL_READSHADER_H
