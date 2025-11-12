#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

// Forward declaration to avoid circular dependency
class Entity;

/**
 * @brief Base class for all components in the Entity-Component System
 * 
 * Components are data containers that can be attached to entities.
 * Each component should inherit from this class and implement specific functionality.
 */
class Component {
protected:
    std::weak_ptr<Entity> m_entity;  // Weak reference to avoid circular dependency
    
public:
    virtual ~Component() = default;
    
    /**
     * @brief Set the entity that owns this component
     * @param entity Weak pointer to the owning entity
     */
    virtual void setEntity(std::weak_ptr<Entity> entity) {
        m_entity = entity;
    }
    
    /**
     * @brief Get the entity that owns this component
     * @return Weak pointer to the owning entity
     */
    std::weak_ptr<Entity> getEntity() const {
        return m_entity;
    }
    
    /**
     * @brief Initialize component (called after entity is set)
     */
    virtual void init() {}
    
    /**
     * @brief Update component (called every frame)
     */
    virtual void update() {}
};

#endif // COMPONENT_H
