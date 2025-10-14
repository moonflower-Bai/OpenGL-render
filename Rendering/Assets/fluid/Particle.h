//
// Created by jingrenbai on 25-3-14.
//

#ifndef PARTICLE_H
#define PARTICLE_H

#include <Eigen/Eigen>
#include <vector>

class Particle {
public:
	Particle() = default;
	Particle(Eigen::Vector3f pos, Eigen::Vector3f vel = Eigen::Vector3f::Zero(), float density = 1.0f, int _h = 100);
    Eigen::Vector3f pos; // 粒子位置
    Eigen::Vector3f vel; // 粒子速度
    float density; // 粒子密度（使用sph方法）
	int _h; // 最大邻居数
	std::vector<Particle*> neighbour; // 邻居粒子
	float lambda; // 拉格朗日乘子
	Eigen::Vector3f oldPos;
	~Particle() = default;

	bool operator==(const Particle& rhs) const;
};


#endif //PARTICLE_H
