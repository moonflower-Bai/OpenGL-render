//
// Created by Jingren Bai on 25-7-16.
//

#include "Camera.hpp"
#include "utils/log.cpp"

Camera::Camera(Eigen::Vector3f cameraPosition,
			   Eigen::Vector3f cameraTarget,
			   Eigen::Vector3f up,
			   float fov, float aspect, float near, float far) :
			   cameraPosition(cameraPosition),
			   cameraTarget(cameraTarget),
			   worldUp(up),
			   fov(fov), aspect(aspect), znear(near), zfar(far) {}
Eigen::Matrix4f Camera::getProjectionMatrix() {
	if(projectionMatrixDirty) updateProjectionMatrix();
	return projectionMatrix;
}
Eigen::Matrix4f Camera::getViewMatrix(){
	if(viewMatrixDirty) updateViewMatrix();
	return viewMatrix;
}

void Camera::setCameraPosition(Eigen::Vector3f cameraPosition) {
	if(cameraPosition != this->cameraPosition){
		this->cameraPosition = cameraPosition;
		viewMatrixDirty = 1;
	}
}
void Camera::setCameraTarget(Eigen::Vector3f cameraTarget) {
	if(cameraTarget != this->cameraTarget){
		this->cameraTarget = cameraTarget;
		viewMatrixDirty = 1;
	}
}
void Camera::setWorldUp(Eigen::Vector3f up) {
	if(up != worldUp){
		worldUp = up;
		viewMatrixDirty = 1;
	}
}
void Camera::setParameters(float fov, float aspect, float near, float far) {
	if(fov == this->fov && aspect == this->aspect && near == znear && far == zfar){
		return;
	}
	this->fov = fov;
	this->aspect = aspect;
	znear = near;
	zfar = far;
	projectionMatrixDirty = 1;
}

void Camera::updateViewMatrix() const {
	Eigen::Vector3f direction = (cameraPosition - cameraTarget).normalized();
	Eigen::Vector3f right = direction.cross(worldUp).normalized();
	Eigen::Vector3f cameraUp = right.cross(direction).normalized();
	Eigen::Matrix4f rotation;
	rotation <<
		right.x(), right.y(), right.z(), 0,
		cameraUp.x(), cameraUp.y(), cameraUp.z(), 0,
		direction.x(), direction.y(), direction.z(), 0,
		0, 0, 0, 1;
	Eigen::Matrix4f translation;
	translation <<
		1, 0, 0, -cameraPosition.x(),
		0, 1, 0, -cameraPosition.y(),
		0, 0, 1, -cameraPosition.z(),
		0, 0, 0, 1;
	viewMatrix = rotation * translation;
	viewMatrixDirty = 0;
}
void Camera::updateProjectionMatrix() const {
	float M_PI = 3.14159265358979323846;
	float alpha = std::tan(fov / 2 * M_PI / 180);
	float n = -znear, f = -zfar;
	float top = alpha * abs(n);
	float bottom = -top;
	float right = top * aspect;
	float left = -right;
	Eigen::Matrix4f ortho;
	ortho <<
		2.0f / (right - left), 0, 0, 0,
		0, 2.0f / (top - bottom), 0, 0,
		0, 0, -2.0f / (n - f), 0,
		0, 0, 0, 1;
	Eigen::Matrix4f translate;
	translate <<
		1, 0, 0, -(left + right) / 2,
		0, 1, 0, -(top + bottom) / 2,
		0, 0, 1, -(f + n) / 2,
		0, 0, 0, 1;
	Eigen::Matrix4f perspective;
	perspective <<
		n, 0, 0, 0,
		0, n, 0, 0,
		0, 0, f + n, -f * n,
		0, 0, 1, 0;
	projectionMatrix = -(ortho * translate) * perspective;
	projectionMatrixDirty = 0;
}