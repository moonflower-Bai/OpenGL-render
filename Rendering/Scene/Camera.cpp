//
// Created by Jingren Bai on 25-7-16.
//

#include "Camera.hpp"
void Camera::update() {
	updateProjectionMatrix();
	updateViewMatrix();
}
void Camera::updateProjectionMatrix() {
	projectionMatrix = Eigen::Matrix4f::Identity();
	float f = std::tan(fov / 2);
	float t = f * znear;
	float b = -t;
	float r = t * aspect;
	float l = -r;
	projectionMatrix <<
					 2 * znear / (r - l), 0, (r + l) / (r - l), 0,
		0, 2 * znear / (t - b), (t + b) / (t - b), 0,
		0, 0, -(zfar + znear) / (zfar - znear), -2 * zfar * znear / (zfar - znear),
		0, 0, -1, 0
	;
}
void Camera::updateViewMatrix() {
	viewMatrix = Eigen::Matrix4f::Identity();

}