//
// Created by Jingren Bai on 25-10-29.
//

#include "Component.h"
#include "Src/Rendering/Pipeline/RenderThread_ECS.h"

TransformComponent::TransformComponent(Eigen::Vector4f position, Quaternions rotation, Eigen::Vector4f scale)
	: m_position(position), m_rotation(rotation), m_scale(scale) {}

void Component::SetEntity(std::shared_ptr<Entity> entity) {
    m_entity = std::move(entity);
}

void Component::setEntity(std::shared_ptr<Entity> entity) {
    SetEntity(std::move(entity));
}

std::shared_ptr<Entity> Component::GetEntity() const {
    return m_entity.lock();
}

std::shared_ptr<Entity> Component::getEntity() const {
    return GetEntity();
}

void Component::tick(float deltaTime) {
	if (!RenderThread_ECS::isGLReady()) {
		static bool warned = false;
		if (!warned) {
			std::cerr << "[Component] Waiting for OpenGL to initialize...\n";
			warned = true;
		}
		return;
	}

	if (!m_started) {
		onStart();
		m_started = true;
	}
	Update(deltaTime);
}