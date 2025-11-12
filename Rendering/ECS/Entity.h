#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <algorithm>
#include "Component.h"

/**
 * @brief Entity class representing game objects in the ECS
 * 
 * Entities are containers for components. They provide methods to add, remove,
 * and retrieve components. Entities use enable_shared_from_this to safely
 * pass themselves to components.
 */
class Entity : public std::enable_shared_from_this<Entity> {
private:
    std::vector<std::shared_ptr<Component>> m_components;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_componentMap;
    
public:
    Entity() = default;
    virtual ~Entity() = default;
    
    /**
     * @brief Add a component to this entity
     * @tparam T Component type (must inherit from Component)
     * @tparam Args Constructor argument types
     * @param args Arguments to forward to the component constructor
     * @return Shared pointer to the created component
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, 
                      "T must inherit from Component");
        
        // Create the component
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        
        // Set the entity reference
        component->setEntity(weak_from_this());
        
        // Store in both vector and map
        m_components.push_back(component);
        m_componentMap[std::type_index(typeid(T))] = component;
        
        // Initialize the component
        component->init();
        
        return component;
    }
    
    /**
     * @brief Get a component of specific type
     * @tparam T Component type to retrieve
     * @return Shared pointer to the component, or nullptr if not found
     */
    template<typename T>
    std::shared_ptr<T> getComponent() {
        static_assert(std::is_base_of<Component, T>::value, 
                      "T must inherit from Component");
        
        auto it = m_componentMap.find(std::type_index(typeid(T)));
        if (it != m_componentMap.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }
    
    /**
     * @brief Check if entity has a component of specific type
     * @tparam T Component type to check
     * @return True if component exists, false otherwise
     */
    template<typename T>
    bool hasComponent() const {
        return m_componentMap.find(std::type_index(typeid(T))) != m_componentMap.end();
    }
    
    /**
     * @brief Remove a component of specific type
     * @tparam T Component type to remove
     */
    template<typename T>
    void removeComponent() {
        auto it = m_componentMap.find(std::type_index(typeid(T)));
        if (it != m_componentMap.end()) {
            // Remove from map
            m_componentMap.erase(it);
            
            // Remove from vector
            m_components.erase(
                std::remove_if(m_components.begin(), m_components.end(),
                    [&it](const std::shared_ptr<Component>& comp) {
                        return comp == it->second;
                    }),
                m_components.end()
            );
        }
    }
    
    /**
     * @brief Get all components
     * @return Vector of all components
     */
    const std::vector<std::shared_ptr<Component>>& getComponents() const {
        return m_components;
    }
    
    /**
     * @brief Update all components
     */
    void update() {
        for (auto& component : m_components) {
            component->update();
        }
    }
};

#endif // ENTITY_H
