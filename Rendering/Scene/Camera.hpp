//
// Created by Jingren Bai on 25-7-16.
//

#ifndef LEARNOPENGL_CAMERA_HPP
#define LEARNOPENGL_CAMERA_HPP

#include "cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/Eigen/Eigen"

class Camera {
private:
	Eigen::Matrix4d projectionMatrix = Eigen::Matrix4d::Identity();
	Eigen::Matrix4d viewMatrix = Eigen::Matrix4d::Identity();
	Eigen::Matrix4d projectionViewMatrix = Eigen::Matrix4d::Identity();
protected:
	Eigen::Vector3d cameraPosition; // 相机位置
	Eigen::Vector3d cameraDirection; // 朝向
	Eigen::Vector3d cameraUp;

	double m_fov = 45.0;
	double m_near = 0.1;
	double m_far = 100.0;
public:
	Camera(Eigen::Vector3d cameraPosition, Eigen::Vector3d cameraDirection, Eigen::Vector3d cameraUp);
	Eigen::Vector3d getCameraPosition();
	Eigen::Vector3d getCameraDirection();
	Eigen::Vector3d getCameraUp();
	void setCameraPosition(Eigen::Vector3d cameraPosition);
	void setCameraDirection(Eigen::Vector3d cameraDirection);
	void setCameraUp(Eigen::Vector3d cameraUp);
	Eigen::Matrix4d getProjectionViewMatrix();
	Eigen::Matrix4d getViewMatrix();
	Eigen::Matrix4d getProjectionMatrix();

public:
	Eigen::Matrix4d getProjectionMatrix(double fov, double aspect, double near, double far);
	void setLookAt(Eigen::Vector3d cameraPosition, Eigen::Vector3d cameraTarget, Eigen::Vector3d cameraUp);
};


#endif //LEARNOPENGL_CAMERA_HPP
