//
// Created by Jingren Bai on 25-9-6.
//

#ifndef LEARNOPENGL_MODEL_H
#define LEARNOPENGL_MODEL_H

#include "cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/Eigen/Eigen"
#include <optional>

#include "Quaternions.h"

class Model {
public:
	static Eigen::Matrix4f getScale(Eigen::Vector3f scale);
	static Eigen::Matrix4f getRotation(Quaternions rotation);
	static Eigen::Matrix<float, 4, 4, Eigen::RowMajor> getTranslate(Eigen::Vector4f position);
};




#endif //LEARNOPENGL_MODEL_H
