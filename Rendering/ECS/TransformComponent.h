#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include <Eigen/Eigen>

/**
 * @brief Component representing position, rotation, and scale
 * 
 * TransformComponent stores the transformation data for an entity,
 * including position, rotation (as Euler angles), and scale.
 */
class TransformComponent : public Component {
private:
    Eigen::Vector3f m_position;
    Eigen::Vector3f m_rotation;  // Euler angles in degrees
    Eigen::Vector3f m_scale;
    
public:
    /**
     * @brief Default constructor
     */
    TransformComponent() 
        : m_position(0.0f, 0.0f, 0.0f)
        , m_rotation(0.0f, 0.0f, 0.0f)
        , m_scale(1.0f, 1.0f, 1.0f) {}
    
    /**
     * @brief Constructor with position
     */
    TransformComponent(const Eigen::Vector3f& position)
        : m_position(position)
        , m_rotation(0.0f, 0.0f, 0.0f)
        , m_scale(1.0f, 1.0f, 1.0f) {}
    
    /**
     * @brief Constructor with position and rotation
     */
    TransformComponent(const Eigen::Vector3f& position, const Eigen::Vector3f& rotation)
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(1.0f, 1.0f, 1.0f) {}
    
    /**
     * @brief Constructor with position, rotation, and scale
     */
    TransformComponent(const Eigen::Vector3f& position, 
                      const Eigen::Vector3f& rotation,
                      const Eigen::Vector3f& scale)
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(scale) {}
    
    virtual ~TransformComponent() = default;
    
    // Position getters and setters
    const Eigen::Vector3f& getPosition() const { return m_position; }
    void setPosition(const Eigen::Vector3f& position) { m_position = position; }
    void setPosition(float x, float y, float z) { m_position = Eigen::Vector3f(x, y, z); }
    
    // Rotation getters and setters
    const Eigen::Vector3f& getRotation() const { return m_rotation; }
    void setRotation(const Eigen::Vector3f& rotation) { m_rotation = rotation; }
    void setRotation(float x, float y, float z) { m_rotation = Eigen::Vector3f(x, y, z); }
    
    // Scale getters and setters
    const Eigen::Vector3f& getScale() const { return m_scale; }
    void setScale(const Eigen::Vector3f& scale) { m_scale = scale; }
    void setScale(float x, float y, float z) { m_scale = Eigen::Vector3f(x, y, z); }
    void setScale(float uniformScale) { m_scale = Eigen::Vector3f(uniformScale, uniformScale, uniformScale); }
    
    /**
     * @brief Get transformation matrix
     * @return 4x4 transformation matrix
     */
    Eigen::Matrix4f getTransformMatrix() const {
        Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();
        
        // Apply scale
        Eigen::Matrix4f scaleMatrix = Eigen::Matrix4f::Identity();
        scaleMatrix(0, 0) = m_scale.x();
        scaleMatrix(1, 1) = m_scale.y();
        scaleMatrix(2, 2) = m_scale.z();
        
        // Apply rotation (simplified - Euler angles)
        float radX = m_rotation.x() * M_PI / 180.0f;
        float radY = m_rotation.y() * M_PI / 180.0f;
        float radZ = m_rotation.z() * M_PI / 180.0f;
        
        Eigen::Matrix4f rotX = Eigen::Matrix4f::Identity();
        rotX(1, 1) = cos(radX); rotX(1, 2) = -sin(radX);
        rotX(2, 1) = sin(radX); rotX(2, 2) = cos(radX);
        
        Eigen::Matrix4f rotY = Eigen::Matrix4f::Identity();
        rotY(0, 0) = cos(radY);  rotY(0, 2) = sin(radY);
        rotY(2, 0) = -sin(radY); rotY(2, 2) = cos(radY);
        
        Eigen::Matrix4f rotZ = Eigen::Matrix4f::Identity();
        rotZ(0, 0) = cos(radZ); rotZ(0, 1) = -sin(radZ);
        rotZ(1, 0) = sin(radZ); rotZ(1, 1) = cos(radZ);
        
        // Apply translation
        Eigen::Matrix4f translation = Eigen::Matrix4f::Identity();
        translation(0, 3) = m_position.x();
        translation(1, 3) = m_position.y();
        translation(2, 3) = m_position.z();
        
        // Combine: T * R * S
        transform = translation * rotZ * rotY * rotX * scaleMatrix;
        
        return transform;
    }
};

#endif // TRANSFORM_COMPONENT_H
