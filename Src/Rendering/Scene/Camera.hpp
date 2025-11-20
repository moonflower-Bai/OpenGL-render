//
// Created by Jingren Bai on 25-7-16.
//

#ifndef LEARNOPENGL_CAMERA_HPP
#define LEARNOPENGL_CAMERA_HPP

#include "cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/Eigen/Eigen"

class Camera {
private:
	Eigen::Vector3f cameraTarget;
	Eigen::Vector3f cameraPosition;
	Eigen::Vector3f worldUp;

	float fov, aspect, znear, zfar;

	mutable Eigen::Matrix4f projectionMatrix;
	mutable Eigen::Matrix4f viewMatrix;
	mutable bool projectionMatrixDirty = true;
	mutable bool viewMatrixDirty = true;

	void updateProjectionMatrix() const;
	void updateViewMatrix() const;
public:
	Camera() = default;
	Camera(Eigen::Vector3f cameraPosition, Eigen::Vector3f cameraTarget, Eigen::Vector3f up, float fov, float aspect, float near, float far);
	Eigen::Matrix4f getProjectionMatrix(); // 只返回，不做计算
	Eigen::Matrix4f getViewMatrix(); // 只返回，不做计算
	void setCameraPosition(Eigen::Vector3f cameraPosition); // 更新参数后调用update()
	void setCameraTarget(Eigen::Vector3f cameraTarget);
	void setWorldUp(Eigen::Vector3f up);
	void setParameters(float fov, float aspect, float near, float far); // 遇到参数更新时调用update()

public:
};


#endif //LEARNOPENGL_CAMERA_HPP
