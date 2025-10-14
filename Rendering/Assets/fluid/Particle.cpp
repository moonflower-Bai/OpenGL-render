//
// Created by jingrenbai on 25-3-14.
//

#include "Particle.h"

Particle::Particle(Eigen::Vector3f pos, Eigen::Vector3f vel, float density, int _h) {
	this->pos = pos;
	this->vel = vel;
	this->density = density;
	this->_h = _h;
}
bool Particle::operator==(const Particle &rhs) const {
    return pos.isApprox(rhs.pos) &&
		   vel.isApprox(rhs.vel) &&
	       density == rhs.density;
}
