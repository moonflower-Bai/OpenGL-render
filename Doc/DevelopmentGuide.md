# LearnOpenGL 开发文档

## 1. 项目概述

这是一个基于 OpenGL 的学习项目，主要用于探索和实践现代 OpenGL 图形编程技术。该项目实现了粒子系统模拟，特别是基于位置的流体（Position Based Fluids, PBF）模拟，并通过 OpenGL 进行可视化渲染。

### 主要特性：
- 基于 Position Based Fluids (PBF) 的流体模拟
- OpenGL 渲染管线
- 多线程渲染架构
- 粒子系统可视化
- 相机控制系统

## 2. 技术栈

- C++20 标准
- OpenGL 3.3+
- GLFW（窗口和输入管理）
- GLAD（OpenGL 函数加载）
- GLM（数学运算）
- Eigen（线性代数运算）
- CMake（构建系统）

## 3. 项目结构

```
LearnOpenGL/
├── Doc/                  # 文档目录
├── Extern/               # 第三方库源码
├── Input/                # 输入系统
├── Rendering/            # 渲染相关代码
│   ├── Assets/           # 资源文件（模型、纹理等）
│   ├── Core/             # 核心渲染组件
│   ├── Pipeline/         # 渲染管线
│   ├── Primitives/       # 基本图元渲染
│   └── Scene/            # 场景相关（相机、模型等）
├── Shader/               # 着色器文件
├── Texture/              # 纹理资源
├── unit_test/            # 单元测试
├── utils/                # 工具类
├── CMakeLists.txt        # CMake 构建配置
└── main.cpp              # 程序入口
```

## 4. 模块详细文档

为了更好地理解和维护项目，我们为每个核心模块提供了详细的独立文档：

- [渲染管线 (Pipeline)](file:///D:/Code/LearnOpenGL/Doc/Pipeline.md) - 管理渲染过程的核心逻辑
- [基本图元 (Primitives)](file:///D:/Code/LearnOpenGL/Doc/Primitives.md) - 实现各种可渲染对象的具体逻辑
- [着色器 (Shader)](file:///D:/Code/LearnOpenGL/Doc/Shader.md) - 着色器程序的加载、编译和管理
- [核心组件 (Core)](file:///D:/Code/LearnOpenGL/Doc/Core.md) - 基础数据结构和功能组件
- [流体模拟 (Fluid)](file:///D:/Code/LearnOpenGL/Doc/Fluid.md) - 基于位置的流体模拟算法实现

## 5. 构建与运行

### 环境要求
- Windows 10/11 或 Linux 系统
- Visual Studio 2019+ 或 GCC 9+（支持 C++20）
- CMake 3.20+
- vcpkg 包管理器

### 构建步骤
1. 安装 vcpkg 并集成到系统中
2. 使用 vcpkg 安装依赖项：
   ```
   vcpkg install glfw3 glm eigen3
   ```
3. 配置 CMake 项目：
   ```
   mkdir build && cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
   ```
4. 编译项目：
   ```
   cmake --build .
   ```

## 6. 使用示例

在 [main.cpp](file:///D:/Code/LearnOpenGL/main.cpp) 中展示了如何使用该框架创建和渲染粒子系统：

```cpp
// 创建相机
Camera camera({0, 0, 3.0f}, {0.0f, 0.0f, 0.0f}, {0, 1, 0},
       45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

// 创建粒子渲染器
auto ptr = std::make_unique<PointRender>(25000, 
    "Shader/Points/Point.vert", 
    "Shader/Points/Point.frag");

ptr->setCamera(camera);
MainRenderThread::add(std::move(ptr));

// 启动渲染循环
MainRenderThread::instance().waitForExit();
```

## 7. 扩展开发

### 添加新的渲染对象
1. 继承 [ObjectRender](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L14-L37) 类
2. 实现 [init()](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L26-L26)、[update()](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L27-L27)、[render()](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L28-L28) 方法
3. 通过 [MainRenderThread::add()](file:///D:/Code/LearnOpenGL/Rendering/Pipeline/MainRenderThread.h#L34-L34) 添加到渲染队列

### 添加新的着色器
1. 在 [Shader](file:///D:/Code/LearnOpenGL/Shader) 目录下创建顶点和片段着色器
2. 在渲染对象中加载并使用新着色器

## 8. 性能优化建议

- 使用顶点缓冲对象（VBO）进行批量渲染
- 合理使用 OpenGL 状态缓存
- 减少 CPU 和 GPU 之间的数据传输
- 使用实例化渲染减少绘制调用次数