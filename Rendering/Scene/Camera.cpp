//
// Created by Jingren Bai on 25-7-16.
//

#include "Camera.hpp"

Camera::Camera(Eigen::Vector3d cameraPosition, Eigen::Vector3d cameraDirection, Eigen::Vector3d cameraUp) {
	this->cameraPosition = cameraPosition;
	this->cameraDirection = cameraDirection;
	this->cameraUp = cameraUp;
	setLookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
}

#pragma region Getters and Setters
Eigen::Vector3d Camera::getCameraDirection() {
	return cameraDirection;
}
Eigen::Vector3d Camera::getCameraPosition() {
	return cameraPosition;
}
Eigen::Vector3d Camera::getCameraUp() {
	return cameraUp;
}

void Camera::setCameraDirection(Eigen::Vector3d cameraDirection) {
	this->cameraDirection = cameraDirection;
}
void Camera::setCameraPosition(Eigen::Vector3d cameraPosition) {
	this->cameraPosition = cameraPosition;
}
void Camera::setCameraUp(Eigen::Vector3d cameraUp) {
	this->cameraUp = cameraUp;
}

Eigen::Matrix4d Camera::getProjectionMatrix() {
	return projectionMatrix;
}
Eigen::Matrix4d Camera::getViewMatrix() {
	return viewMatrix;
}

#pragma endregion

#pragma region Projection and View
Eigen::Matrix4d Camera::getProjectionViewMatrix() {
	return projectionViewMatrix = projectionMatrix * viewMatrix;
}

Eigen::Matrix4d Camera::getProjectionMatrix(double fov, double aspect, double near, double far) {
	Eigen::Matrix4d Ortho = Eigen::Matrix4d::Identity();
	double f = static_cast<double>(std::tan(fov / 2.0));
	double t = f * near;
	double r = t * aspect;
	Ortho <<
		1.0 / r, 0.0, 0.0, 0.0,
		0.0, 1.0 / t, 0.0, 0.0,
		0.0, 0.0, -2.0 / (far - near), -(far + near) / 2,
		0.0, 0.0, 0.0, 1.0;
//	Eigen::Matrix4d Trans;
//	Trans << 1.0, 0.0, 0.0, 0.0,
//		0.0, 1.0, 0.0, 0.0,
//		0.0, 0.0, 1.0, -(far + near) / 2.0,
//		0.0, 0.0, 0.0, 1.0;
//	Ortho = Ortho * Trans;

	Eigen::Matrix4d Prespective;
	Prespective <<
		near, 0.0, 0.0, 0.0,
		0.0, near, 0.0, 0.0,
		0.0, 0.0, far + near, -1.0 * far * near,
		0.0, 0.0, 1.0, 0.0;

	projectionMatrix = Ortho * Prespective * projectionMatrix;
	return projectionMatrix;
}

void Camera::setLookAt(Eigen::Vector3d cameraPosition, Eigen::Vector3d cameraTarget, Eigen::Vector3d cameraUp) {
	setCameraPosition(cameraPosition);
	setCameraDirection(cameraPosition - cameraTarget);
	setCameraUp(cameraUp);

	Eigen::Matrix4d Translation;
	Translation << 1.0, 0.0, 0.0, -cameraPosition[0],
		0.0, 1.0, 0.0, -cameraPosition[1],
		0.0, 0.0, 1.0, -cameraPosition[2],
		0.0, 0.0, 0.0, 1.0;

	Eigen::Vector3d GxT = cameraTarget.cross(cameraUp);
	Eigen::Matrix4d Rotation;
	Rotation << GxT[0], cameraUp[0], cameraTarget[0], 0.0,
		GxT[1], cameraUp[1], cameraTarget[1], 0.0,
		GxT[2], cameraUp[2], cameraTarget[2], 0.0,
		0.0, 0.0, 0.0, 1.0;
	viewMatrix = Rotation * Translation;
}
#pragma endregion