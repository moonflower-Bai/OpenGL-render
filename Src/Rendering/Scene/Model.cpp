//
// Created by Jingren Bai on 25-9-6.
//

#include "Model.h"

#pragma region matrix getter with parameter
Eigen::Matrix4f Model::getScale(Eigen::Vector3f scale) {
	Eigen::Matrix4f scaleMatrix;
	scaleMatrix <<
			scale.x(), 0, 0, 0,
			0, scale.y(), 0, 0,
			0, 0, scale.z(), 0,
			0, 0, 0, 1;
	return scaleMatrix;
}

Eigen::Matrix4f Model::getRotation(Quaternions rotation) {
	return rotation.getRotationMatrix();
}

Eigen::Matrix<float, 4, 4, Eigen::RowMajor> Model::getTranslate(Eigen::Vector4f _position){
	Eigen::Matrix<float, 4, 4, Eigen::RowMajor> translateMatrix;
	translateMatrix <<
			1, 0, 0, _position.x(),
			0, 1, 0, _position.y(),
			0, 0, 1, _position.z(),
			0, 0, 0, _position.w();
	return translateMatrix;
}
#pragma endregion
