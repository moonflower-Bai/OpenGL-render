//
// Created by Jingren Bai on 25-9-8.
//

#ifndef LEARNOPENGL_QUATERNIONS_H
#define LEARNOPENGL_QUATERNIONS_H

#include <Eigen/Eigen>

class Quaternions {
private:
	float w, x, y, z;
public:
	Quaternions(float w, float x, float y, float z);
	explicit Quaternions(Eigen::Vector4f quaternion);
	Quaternions(float angle, Eigen::Vector3f axis);
	Eigen::Matrix4f getRotationMatrix();
};

#endif //LEARNOPENGL_QUATERNIONS_H
