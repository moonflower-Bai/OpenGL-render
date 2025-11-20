//
// Created by Jingren Bai on 25-8-11.
//

#ifndef LEARNOPENGL_VERTEX_H
#define LEARNOPENGL_VERTEX_H

#include "cmake-build-debug-visual-studio/vcpkg_installed/x64-windows/include/Eigen/Eigen"
#include "Rendering/Assets/fluid/CPU_process/Particle.h"

struct alignas(16) Vertex {
	Vertex() = default;
	Vertex(Eigen::Vector4f color, Eigen::Vector4f position, Eigen::Vector2f texCoord){
		this->color = color;
		this->position = position;
		this->texCoord = texCoord;
	}
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Eigen::Vector4f color;
	Eigen::Vector4f position;
	Eigen::Vector2f texCoord;

	static Vertex particlesToVertex(Particle particle){
//		return {Eigen::Vector4f(fabs(particle.vel.x()), fabs(particle.vel.y()), fabs(particle.vel.z()), 255.0f),
		return {Eigen::Vector4f(1-particle.density, 1-particle.density, 1-particle.density, 1.0f),
//		return {(particle.vel.y() > 0) ? Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f): Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f),
//		return {Eigen::Vector4f(0x66 / 255.0, 0xcc / 255.0, 0xff / 255.0, 1.0f),
					  Eigen::Vector4f(particle.pos.x(), particle.pos.y(), -particle.pos.z(), 10.0f),
					  Eigen::Vector2f(0, 0)};
	}
};


#endif //LEARNOPENGL_VERTEX_H
