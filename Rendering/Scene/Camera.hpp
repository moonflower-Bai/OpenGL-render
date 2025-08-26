//
// Created by Jingren Bai on 25-7-16.
//

#ifndef LEARNOPENGL_CAMERA_HPP
#define LEARNOPENGL_CAMERA_HPP

#include <Eigen/Eigen>

class Camera {
private:
	Eigen::Vector3f cameraDirection;
	Eigen::Vector3f cameraPosition;
	Eigen::Vector3f cameraUp;
	Eigen::Matrix4f projectionMatrix;
	Eigen::Matrix4f viewMatrix;
	float fov, aspect, near, far;
	void updateProjectionMatrix(); // 遇到参数更新时调用
	void updateViewMatrix(); // 遇到参数更新时调用
	void update(); // 同时更新投影矩阵和视图矩阵
	void init(); // 初始化
public:
	Camera();
	Camera(Eigen::Vector3f cameraPosition, Eigen::Vector3f cameraTarget, Eigen::Vector3f cameraUp, float fov, float aspect, float near, float far);
	Eigen::Matrix4f getProjectionMatrix(); // 只返回，不做计算
	Eigen::Matrix4f getViewMatrix(); // 只返回，不做计算
	void setCameraPosition(Eigen::Vector3f cameraPosition); // 更新参数后调用update()
	void setCameraDirection(Eigen::Vector3f cameraDirection);
	void setCameraUp(Eigen::Vector3f cameraUp);
	void setParameters(float fov, float aspect, float near, float far); // 遇到参数更新时调用update()
};


#endif //LEARNOPENGL_CAMERA_HPP
