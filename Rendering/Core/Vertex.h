//
// Created by Jingren Bai on 25-8-11.
//

#ifndef LEARNOPENGL_VERTEX_H
#define LEARNOPENGL_VERTEX_H

#include <Eigen/Eigen>

struct alignas(16) Vertex {
	Vertex(Eigen::Vector4f color, Eigen::Vector3f position, Eigen::Vector2f texCoord){
		this->color = color;
		this->position = position;
		this->texCoord = texCoord;
	}
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Eigen::Vector4f color;
	Eigen::Vector3f position;
	Eigen::Vector2f texCoord;
};


#endif //LEARNOPENGL_VERTEX_H
