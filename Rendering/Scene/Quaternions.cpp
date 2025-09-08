//
// Created by Jingren Bai on 25-9-8.
//

#include "Quaternions.h"

Quaternions::Quaternions(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

Quaternions::Quaternions(Eigen::Vector4f quaternion) : w(quaternion(0)), x(quaternion(1)), y(quaternion(2)), z(quaternion(3)) {}

Quaternions::Quaternions(float angle, Eigen::Vector3f axis){
	angle = angle / 2;
	axis = axis.normalized();
	w = cos(angle);
	x = axis(0) * sin(angle);
	y = axis(1) * sin(angle);
	z = axis(2) * sin(angle);
}
Eigen::Matrix4f Quaternions::getRotationMatrix() {
	Eigen::Matrix4f rotationMatrix;
	rotationMatrix <<
			w * w + x * x - y * y - z * z, 2 * (x * y - w * z), 2 * (x * z + w * y), 0,
			2 * (x * y + w * z), w * w - x * x + y * y - z * z, 2 * (y * z - w * x), 0,
			2 * (x * z - w * y), 2 * (y * z + w * x), w * w - x * x - y * y + z * z, 0,
			0, 0, 0, 1;
	return rotationMatrix;
}