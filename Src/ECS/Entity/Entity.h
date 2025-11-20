//
// Created by Jingren Bai on 25-10-29.
//

#ifndef LEARNOPENGL_ENTITY_H
#define LEARNOPENGL_ENTITY_H

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "ECS/Components/Component.h"

class Entity : public std::enable_shared_from_this<Entity> {
private:
	std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
public:
	Entity() = default;
	~Entity() = default;
// ---- 添加组件 ----
	template<typename T, typename... Args>
	std::shared_ptr<T> addComponent(Args&&... args) {
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		auto typeIdx = std::type_index(typeid(T));
		if (components.count(typeIdx))
			throw std::runtime_error("Component already exists in this Entity!");

		auto comp = std::make_shared<T>(std::forward<Args>(args)...);
		comp->setEntity(shared_from_this());
		comp->onAttach();
		components[typeIdx] = comp;
		return comp;
	}

	// ---- 获取组件 ----
	template<typename T>
	std::shared_ptr<T> getComponent() const {
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

		auto it = components.find(std::type_index(typeid(T)));
		if (it != components.end())
			return std::dynamic_pointer_cast<T>(it->second);
		return nullptr;
	}

	// ---- 移除组件 ----
	template<typename T>
	void removeComponent() {
		auto typeIdx = std::type_index(typeid(T));
		auto it = components.find(typeIdx);
		if (it != components.end()) {
			it->second->onDetach();
			components.erase(it);
		}
	}

	// ---- 每帧更新所有组件 ----
	void update(float deltaTime) {
		for (auto& [type, comp] : components) {
			comp->tick(deltaTime);  // ✅ 统一生命周期调度
		}
	}
};


#endif //LEARNOPENGL_ENTITY_H
