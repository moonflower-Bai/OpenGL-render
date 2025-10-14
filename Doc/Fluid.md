# 流体模拟 (Fluid) 模块文档

## 1. 模块概述

流体模拟模块实现了基于位置的流体（Position Based Fluids, PBF）模拟算法，用于模拟和渲染流体动力学效果。该模块主要包括粒子类（Particle）和模拟器类（Simulator），为粒子系统渲染提供物理模拟支持。

## 2. 核心类

### 2.1 Particle 类

Particle 类表示单个流体粒子，包含粒子的位置、速度、密度等物理属性。

#### 类定义

```cpp
class Particle {
public:
    Particle() = default;
    Particle(Eigen::Vector3f pos, Eigen::Vector3f vel = Eigen::Vector3f::Zero(), float density = 1.0f, int _h = 100);
    Eigen::Vector3f pos; // 粒子位置
    Eigen::Vector3f vel; // 粒子速度
    float density; // 粒子密度（使用sph方法）
    int _h; // 最大邻居数
    std::vector<Particle*> neighbour; // 邻居粒子
    float lambda; // 拉格朗日乘子
    Eigen::Vector3f oldPos;
    ~Particle() = default;
    bool operator==(const Particle& rhs) const;
};
```

#### 成员变量说明

- [pos](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/Particle.h#L13-L13): 粒子在三维空间中的位置
- [vel](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/Particle.h#L14-L14): 粒子的速度向量
- [density](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/Particle.h#L15-L15): 粒子密度（使用SPH方法计算）
- [_h](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/Particle.h#L16-L16): 最大邻居数
- [neighbour](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/Particle.h#L17-L17): 邻居粒子列表
- [lambda](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/Particle.h#L18-L18): 拉格朗日乘子（用于PBF算法）
- [oldPos](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/Particle.h#L19-L19): 粒子的旧位置

#### 构造函数说明

##### Particle(Eigen::Vector3f pos, Eigen::Vector3f vel = Eigen::Vector3f::Zero(), float density = 1.0f, int _h = 100)
构造函数，创建一个粒子对象。

参数：
- pos: 粒子位置
- vel: 粒子速度，默认为零向量
- density: 粒子密度，默认为1.0
- _h: 最大邻居数，默认为100

### 2.2 Simulator 类

Simulator 类实现了基于位置的流体模拟算法，负责管理粒子集合并执行模拟计算。

#### 类定义

```cpp
class Simulator {
public:
    Simulator() = default;
    explicit Simulator(int particleNums);
    ~Simulator() = default;
    
    void init(int particleNums);
    float poly6Value(float, float h);
    Eigen::Vector3f spikyGradient(Eigen::Vector3f, float r, float h);
    void runPBF();
    void prologue();
    void update();
    void epilogue();
    const std::vector<Particle>& getParticles();
    const std::vector<Eigen::Vector3f>& getParticlePos();
    Eigen::Vector3f getBoundingBox();

private:
    // 参数设置
    int screen_x = 800, screen_y = 600;
    float screenToWorldRatio = 20.0;
    Eigen::Vector3f boundary;
    float epsilon = 1e-5;
    float neighbourRadius = 1.01f;
    
    // 网格坐标计算
    float cellSize = 2.51;
    float cellRecpr = 1.0 / cellSize;
    Eigen::Vector3i gridSize;
    std::vector<Particle*> gridToParticles[2000000];
    int roundUp(float, float);
    
    // 网格参数
    int maxNeighbour = 40;
    
    // 粒子参数
    float dt = 0.05f;
    float particleRadius = 1.1f, particleRadiusInWorld = particleRadius / screenToWorldRatio;
    std::vector<Particle> particles;
    std::vector<Eigen::Vector3f> particlePos;
    Eigen::Vector3f gravity;
    
    // 粒子更新
    Eigen::Vector3f confineParticle(Eigen::Vector3f pos);
    Eigen::Vector3i getCell(Eigen::Vector3f pos);
    
    // PBF参数
    int pbfNumIters = 5;
    float h = 1.1;
    float mass = 1.0, rho = 1.0;
    float lambdaEpsilon = 100.0;
    float corrDeltaQCooff = 0.3f, corrK = 0.001;
    
    bool isInRange(const Eigen::Vector3i &cell);
    unsigned int computeMortonCode(Eigen::Vector3f pos);
};
```

#### 核心属性说明

##### 物理参数
- [screen_x, screen_y](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L24-L24): 屏幕尺寸
- [screenToWorldRatio](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L25-L25): 屏幕坐标与世界坐标的比率
- [boundary](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L26-L26): 模拟边界大小
- [epsilon](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L27-L27): 浮点数误差阈值
- [neighbourRadius](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L28-L28): 邻居搜索半径

##### 网格参数
- [cellSize](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L31-L31): 网格单元大小
- [cellRecpr](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L32-L32): 网格单元大小的倒数
- [gridSize](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L33-L33): 网格尺寸
- [gridToParticles](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L35-L35): 网格到粒子的映射

##### 粒子参数
- [dt](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L45-L45): 时间步长
- [particleRadius](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L46-L46): 粒子半径
- [particles](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L48-L48): 粒子集合
- [particlePos](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L50-L50): 粒子位置集合
- [gravity](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L51-L51): 重力向量

##### PBF算法参数
- [pbfNumIters](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L55-L55): PBF迭代次数
- [h](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L56-L56): 核函数半径
- [mass](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L57-L57): 粒子质量
- [rho](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L57-L57): 粒子静止密度
- [lambdaEpsilon](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L58-L58): 拉格朗日乘子计算参数
- [corrDeltaQCooff, corrK](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L63-L63): XSPH修正参数

#### 核心方法说明

##### Simulator(int particleNums)
构造函数，初始化模拟器并创建指定数量的粒子。

参数：
- particleNums: 粒子数量

##### runPBF()
执行PBF算法的主要入口函数，进行一次完整的流体模拟迭代。

工作流程：
1. 调用 [prologue()](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L39-L39) 初始化粒子状态
2. 执行 [pbfNumIters](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L55-L55) 次PBF迭代
3. 调用 [epilogue()](file:///D:/Code/LearnOpenGL/Rendering/Assets/fluid/FluidSimulator.h#L41-L41) 更新粒子最终状态

##### prologue()
初始化粒子状态，包括：
1. 根据重力和速度更新粒子位置
2. 将粒子位置限制在边界内
3. 构建空间网格以加速邻居搜索

##### update()
更新粒子状态，包括：
1. 计算粒子密度
2. 计算拉格朗日乘子
3. 计算位置修正量
4. 应用位置修正

##### epilogue()
更新粒子最终状态，包括：
1. 根据新旧位置计算新速度
2. 应用XSPH修正
3. 更新粒子位置列表

##### getParticles()
获取所有粒子对象的引用。

返回值：
- const std::vector<Particle>&: 粒子对象列表

##### poly6Value(float r, float h)
计算Poly6核函数值。

参数：
- r: 距离
- h: 核函数半径

##### spikyGradient(Eigen::Vector3f r, float r_len, float h)
计算Spiky核函数梯度。

参数：
- r: 距离向量
- r_len: 距离
- h: 核函数半径

## 3. PBF算法简介

基于位置的流体（Position Based Fluids, PBF）是一种用于实时流体模拟的算法，它通过直接修正粒子位置来满足流体约束，避免了传统基于力的方法中的刚性问题。

### 3.1 算法流程

1. 预处理阶段（prologue）
   - 根据当前速度预测新位置
   - 将粒子限制在边界内
   - 构建空间网格以加速邻居搜索

2. 迭代求解阶段
   - 计算每个粒子的密度约束
   - 计算拉格朗日乘子
   - 计算位置修正量
   - 应用位置修正

3. 后处理阶段（epilogue）
   - 根据位置变化计算新速度
   - 应用XSPH修正以提高稳定性

### 3.2 核心公式

密度约束：
```
C_i = (1/ρ₀) * Σ m_j * W(p_i - p_j, h) - 1
```

拉格朗日乘子：
```
λ_i = C_i / (Σ |∇C_j|² + ε)
```

位置修正：
```
Δp_i = -λ_i * ∇C_i
```