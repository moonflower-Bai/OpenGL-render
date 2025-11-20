//
// Created by Jingren Bai on 25-11-13.
//

#ifndef LEARNOPENGL_GPU_PARTICLE_H
#define LEARNOPENGL_GPU_PARTICLE_H

#include <Eigen/Eigen>
#include <vector>

struct GPU_Particle {
	GPU_Particle() = default;
	GPU_Particle(Eigen::Vector3f pos, Eigen::Vector3f vel = Eigen::Vector3f::Zero(), float density = 1.0f){
		this->pos = {pos.x(),pos.y(),pos.z(), 1.0f};
		this->vel = {vel.x(),vel.y(),vel.z(), 0.0f};
		this->density = density;
	}
	Eigen::Vector4f pos; // 粒子位置
	Eigen::Vector4f vel; // 粒子速度
	Eigen::Vector4f oldPos = Eigen::Vector4f::Zero();
	float lambda = 0; // 拉格朗日乘子
	float density = 0; // 粒子密度（使用sph方法）
	float _padA = 0; // padding
	float _padB = 0; // padding
	~GPU_Particle() = default;

	bool operator==(const GPU_Particle& rhs) const = default;
};

#endif //LEARNOPENGL_GPU_PARTICLE_H
