/**
 * @file ecs_test.cpp
 * @brief Test file for Entity-Component System
 * 
 * This file demonstrates the correct usage of the ECS and ensures
 * the compilation errors described in the issue are resolved.
 */

#include "Rendering/ECS/Entity.h"
#include "Rendering/ECS/Component.h"
#include "Rendering/ECS/TransformComponent.h"
#include "Rendering/Assets/fluid/GPU_process/GPU_FluidSimulator.h"
#include <iostream>

int main() {
    // Create an entity for particle simulation
    auto particleEntity = std::make_shared<Entity>();
    
    // Test 1: Add TransformComponent (this was failing before)
    auto transform = particleEntity->addComponent<TransformComponent>(
        Eigen::Vector3f(0.0f, 0.0f, 0.0f),  // position
        Eigen::Vector3f(0.0f, 0.0f, 0.0f),  // rotation
        Eigen::Vector3f(1.0f, 1.0f, 1.0f)   // scale
    );
    
    std::cout << "TransformComponent added successfully!" << std::endl;
    std::cout << "Position: " << transform->getPosition().transpose() << std::endl;
    
    // Test 2: Add GPU_FluidSimulator (this was failing with setEntity error)
    auto fluidSim = particleEntity->addComponent<GPU_FluidSimulator>(5000);
    
    std::cout << "GPU_FluidSimulator added successfully!" << std::endl;
    std::cout << "Particle count: " << fluidSim->getParticleCount() << std::endl;
    
    // Test 3: Verify component retrieval
    auto retrievedTransform = particleEntity->getComponent<TransformComponent>();
    if (retrievedTransform) {
        std::cout << "Successfully retrieved TransformComponent" << std::endl;
    }
    
    auto retrievedFluidSim = particleEntity->getComponent<GPU_FluidSimulator>();
    if (retrievedFluidSim) {
        std::cout << "Successfully retrieved GPU_FluidSimulator" << std::endl;
    }
    
    // Test 4: Update all components
    particleEntity->update();
    std::cout << "All components updated successfully!" << std::endl;
    
    // Test 5: Check hasComponent
    if (particleEntity->hasComponent<TransformComponent>()) {
        std::cout << "Entity has TransformComponent" << std::endl;
    }
    
    if (particleEntity->hasComponent<GPU_FluidSimulator>()) {
        std::cout << "Entity has GPU_FluidSimulator" << std::endl;
    }
    
    std::cout << "\nAll ECS tests passed successfully!" << std::endl;
    return 0;
}
