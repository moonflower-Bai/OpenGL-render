//
// Created by Jingren Bai on 25-8-16.
//

#ifndef LEARNOPENGL_OBJECTCONFIG_H
#define LEARNOPENGL_OBJECTCONFIG_H

#include <string>
#include <Eigen/Eigen>

struct ObjectConfig {
    std::string fragmentShaderPath;
    std::string vertexShaderPath;
    std::string texturePath;
    std::string modelPath;
    std::string modelName;
    Eigen::Vector4i color;
};

#endif //LEARNOPENGL_OBJECTCONFIG_H
