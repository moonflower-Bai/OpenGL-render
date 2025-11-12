# Entity-Component System (ECS) 文档

## 概述

本项目实现了一个简单但功能完整的 Entity-Component System (ECS)，用于组织游戏对象和组件。ECS 是一种常见的游戏架构模式，将数据（组件）与行为分离，提供更好的代码组织和复用性。

## 核心类

### Component (组件基类)

**文件位置**: `Rendering/ECS/Component.h`

Component 是所有组件的基类。每个具体的组件都应该继承这个类。

```cpp
class Component {
protected:
    std::weak_ptr<Entity> m_entity;  // 弱引用，避免循环依赖
    
public:
    virtual ~Component() = default;
    virtual void setEntity(std::weak_ptr<Entity> entity);
    std::weak_ptr<Entity> getEntity() const;
    virtual void init() {}
    virtual void update() {}
};
```

**关键点**:
- 使用 `std::weak_ptr<Entity>` 避免循环引用
- `setEntity` 是虚函数，允许子类重写
- `init()` 在组件添加到实体后调用
- `update()` 每帧调用（如果实现）

### Entity (实体)

**文件位置**: `Rendering/ECS/Entity.h`

Entity 是组件的容器，代表游戏中的对象。

```cpp
class Entity : public std::enable_shared_from_this<Entity> {
public:
    template<typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args&&... args);
    
    template<typename T>
    std::shared_ptr<T> getComponent();
    
    template<typename T>
    bool hasComponent() const;
    
    template<typename T>
    void removeComponent();
    
    void update();
};
```

**主要方法**:

#### addComponent<T>(Args&&... args)
添加组件到实体。

- 使用完美转发构造组件
- 自动调用 `component->setEntity()`
- 自动调用 `component->init()`
- 返回组件的 shared_ptr

**示例**:
```cpp
auto entity = std::make_shared<Entity>();
auto transform = entity->addComponent<TransformComponent>(
    Eigen::Vector3f(0, 0, 0),  // position
    Eigen::Vector3f(0, 0, 0),  // rotation
    Eigen::Vector3f(1, 1, 1)   // scale
);
```

#### getComponent<T>()
获取指定类型的组件。

**示例**:
```cpp
auto transform = entity->getComponent<TransformComponent>();
if (transform) {
    transform->setPosition(1.0f, 2.0f, 3.0f);
}
```

#### hasComponent<T>()
检查实体是否有指定类型的组件。

**示例**:
```cpp
if (entity->hasComponent<TransformComponent>()) {
    // 实体有 TransformComponent
}
```

## 内置组件

### TransformComponent (变换组件)

**文件位置**: `Rendering/ECS/TransformComponent.h`

存储位置、旋转和缩放信息。

```cpp
class TransformComponent : public Component {
public:
    TransformComponent();
    TransformComponent(const Eigen::Vector3f& position);
    TransformComponent(const Eigen::Vector3f& position, 
                      const Eigen::Vector3f& rotation);
    TransformComponent(const Eigen::Vector3f& position, 
                      const Eigen::Vector3f& rotation,
                      const Eigen::Vector3f& scale);
    
    // Position
    const Eigen::Vector3f& getPosition() const;
    void setPosition(const Eigen::Vector3f& position);
    void setPosition(float x, float y, float z);
    
    // Rotation (Euler angles in degrees)
    const Eigen::Vector3f& getRotation() const;
    void setRotation(const Eigen::Vector3f& rotation);
    void setRotation(float x, float y, float z);
    
    // Scale
    const Eigen::Vector3f& getScale() const;
    void setScale(const Eigen::Vector3f& scale);
    void setScale(float x, float y, float z);
    void setScale(float uniformScale);
    
    // Get transformation matrix
    Eigen::Matrix4f getTransformMatrix() const;
};
```

### GPU_FluidSimulator (GPU 流体模拟器组件)

**文件位置**: `Rendering/Assets/fluid/GPU_process/GPU_FluidSimulator.h`

GPU 加速的流体模拟组件。

```cpp
class GPU_FluidSimulator : public Component {
public:
    explicit GPU_FluidSimulator(int particleCount = 1000);
    
    void init() override;
    void update() override;
    
    int getParticleCount() const;
    const std::vector<Eigen::Vector3f>& getPositions() const;
    const std::vector<Eigen::Vector3f>& getVelocities() const;
    
    void setPositions(const std::vector<Eigen::Vector3f>& positions);
    void setVelocities(const std::vector<Eigen::Vector3f>& velocities);
};
```

## 使用示例

### 创建带组件的实体

```cpp
#include "Rendering/ECS/Entity.h"
#include "Rendering/ECS/TransformComponent.h"
#include "Rendering/Assets/fluid/GPU_process/GPU_FluidSimulator.h"

// 创建粒子实体
auto particleEntity = std::make_shared<Entity>();

// 添加变换组件
auto transform = particleEntity->addComponent<TransformComponent>(
    Eigen::Vector3f(0.0f, 0.0f, 0.0f)   // 位置
);

// 添加流体模拟器组件
auto fluidSim = particleEntity->addComponent<GPU_FluidSimulator>(5000);

// 使用组件
transform->setPosition(1.0f, 2.0f, 3.0f);

// 更新所有组件
particleEntity->update();
```

### 创建自定义组件

```cpp
#include "Rendering/ECS/Component.h"

class MyCustomComponent : public Component {
private:
    int m_data;
    
public:
    MyCustomComponent(int data) : m_data(data) {}
    
    void init() override {
        // 在组件添加到实体后调用
        // 可以访问 m_entity
    }
    
    void update() override {
        // 每帧调用
        m_data++;
    }
    
    int getData() const { return m_data; }
};

// 使用自定义组件
auto entity = std::make_shared<Entity>();
auto myComp = entity->addComponent<MyCustomComponent>(42);
```

## 常见错误及解决方案

### 错误 1: "No matching member function for call to 'addComponent'"

**原因**: 使用了花括号 `{}` 而不是圆括号 `()` 传递参数。

**错误示例**:
```cpp
// ❌ 错误
entity->addComponent<TransformComponent>({0, 0, 0});
```

**正确示例**:
```cpp
// ✅ 正确
entity->addComponent<TransformComponent>(Eigen::Vector3f(0, 0, 0));
```

### 错误 2: "no member named 'setEntity' in 'MyComponent'"

**原因**: 组件类没有正确继承 Component 或使用了 private 继承。

**错误示例**:
```cpp
// ❌ 错误 - 默认是 private 继承
class MyComponent : Component { };
```

**正确示例**:
```cpp
// ✅ 正确 - 显式使用 public 继承
class MyComponent : public Component { };
```

### 错误 3: 循环依赖导致的编译错误

**原因**: Component.h 和 Entity.h 相互包含。

**解决方案**: 在 Component.h 中使用前向声明。

```cpp
// Component.h
class Entity;  // 前向声明

class Component {
protected:
    std::weak_ptr<Entity> m_entity;  // 使用 weak_ptr
    // ...
};
```

## 设计考虑

### 为什么使用 weak_ptr?

Component 持有 Entity 的 `weak_ptr` 而不是 `shared_ptr`，这避免了循环引用：
- Entity 持有 Component 的 `shared_ptr`
- Component 持有 Entity 的 `weak_ptr`

这样当 Entity 被销毁时，所有组件也会自动销毁，不会造成内存泄漏。

### 为什么使用 enable_shared_from_this?

Entity 继承 `enable_shared_from_this<Entity>`，这允许在成员函数中安全地获取指向自身的 `shared_ptr`，这在 `addComponent` 中调用 `weak_from_this()` 时需要。

### 类型安全

使用 `std::type_index` 和 `typeid` 确保每个实体只能有一个特定类型的组件实例。

## 编译和测试

### 编译 ECS 测试

```bash
cd /home/runner/work/OpenGL-render/OpenGL-render
g++ -std=c++20 -I. -I/usr/include/eigen3 -o ecs_test unit_test/ecs_test.cpp
./ecs_test
```

### 使用 CMake 构建

```bash
mkdir build && cd build
cmake ..
make ecs_test
./ecs_test
```

## 性能考虑

当前实现优先考虑简单性和易用性。如果需要更高性能：

1. **组件查找**: 使用 `unordered_map` 提供 O(1) 查找
2. **内存局部性**: 考虑实现数据导向设计 (DOD)
3. **批量更新**: 实现系统 (System) 来批量处理相同类型的组件

## 扩展建议

1. **添加 System 概念**: 创建系统来批量处理组件
2. **事件系统**: 添加组件间通信机制
3. **序列化**: 实现组件的保存和加载
4. **组件依赖**: 自动添加依赖的组件

## 参考

- 完整示例: `Doc/ecs_usage_example.cpp`
- 测试代码: `unit_test/ecs_test.cpp`
- Component 基类: `Rendering/ECS/Component.h`
- Entity 类: `Rendering/ECS/Entity.h`
