//
// Created by Jingren Bai on 25-10-29.
//

#ifndef LEARNOPENGL_COMPONENT_H
#define LEARNOPENGL_COMPONENT_H

#include <Eigen/Eigen>
#include <optional>
#include <iostream>

#include "Rendering/Scene/Quaternions.h"

class Entity;
class RenderThread_ECS;

class Component {
	std::weak_ptr<Entity> m_entity;
	bool m_started = false;
public:
    virtual ~Component() = default;

    virtual void Update(float deltaTime) {} // 每帧调用更新组件状态
	virtual void onAttach() {} // 当组件被添加到实体时调用
	virtual void onStart() {} // 在第一次更新前调用, 确保OpenGL上下文已准备好
	virtual void onDetach() {} // 当组件从实体中移除时调用

	void SetEntity(std::shared_ptr<Entity> entity);

	// Compatibility: some code (Entity::addComponent) expects setEntity (lowercase)
	void setEntity(std::shared_ptr<Entity> entity);

	[[nodiscard]] std::shared_ptr<Entity> GetEntity() const;

	// Compatibility wrapper for getEntity
	[[nodiscard]] std::shared_ptr<Entity> getEntity() const;

	void tick(float deltaTime);
};

class TransformComponent : public Component {
    Eigen::Vector4f m_position;
    Quaternions m_rotation;
    Eigen::Vector4f m_scale;
public:
    TransformComponent(Eigen::Vector4f position, Quaternions rotation, Eigen::Vector4f scale);
};

class MeshComponent : public Component {
private:
    std::vector<Eigen::Vector4f> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Eigen::Vector4f> m_normals;
    std::vector<Eigen::Vector2f> m_uvs;

    bool m_hasUVs = false;
public:
    MeshComponent(const std::vector<Eigen::Vector4f>& vertices, const std::vector<unsigned int>& indices)
        : m_vertices(vertices), m_indices(indices) {
        m_hasUVs = false;
        // TODO: compute normals
    }

    MeshComponent(const std::vector<Eigen::Vector4f>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<Eigen::Vector4f>& normals,
        const std::optional<std::vector<Eigen::Vector2f>>& uvs)
        : m_vertices(vertices), m_indices(indices), m_normals(normals) {
        if (uvs.has_value()) {
            m_uvs = uvs.value();
            m_hasUVs = true;
        }
        else{
            m_hasUVs = false;
        }
    }

    [[nodiscard]] const std::vector<Eigen::Vector4f>& GetVertices() const { return m_vertices; }
    [[nodiscard]] const std::vector<unsigned int>& GetIndices() const { return m_indices; }
    [[nodiscard]] const std::vector<Eigen::Vector4f>& GetNormals() const { return m_normals; }
    [[nodiscard]] const std::vector<Eigen::Vector2f>& GetUVs() const {
        if(!m_hasUVs){
            throw std::runtime_error("Mesh does not have UVs");
        }
        return m_uvs;
    }
    [[nodiscard]]bool HasUVs() const { return m_hasUVs; }

};

class MeterialComponent : public Component {
public:
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;

    std::string m_diffuseTexturePath;
    std::string m_specularTexturePath;
    std::string m_ambientTexturePath;

    std::string m_normalTexturePath;

private:
    bool m_hasDiffuseTexture = false;
    bool m_hasSpecularTexture = false;
    bool m_hasAmbientTexture = false;
    bool m_hasNormalTexture = false;



};

class RenderComponent : public Component {

};

#endif //LEARNOPENGL_COMPONENT_H
