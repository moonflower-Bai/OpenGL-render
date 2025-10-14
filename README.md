# LearnOpenGL

这是一个基于 OpenGL 的学习项目，主要用于探索和实践现代 OpenGL 图形编程技术。该项目实现了粒子系统模拟，特别是基于位置的流体（Position Based Fluids, PBF）模拟，并通过 OpenGL 进行可视化渲染。

![Fluid Simulation](Texture/wall.jpg)

## 主要特性

- 基于 Position Based Fluids (PBF) 的流体模拟
- OpenGL 渲染管线
- 多线程渲染架构
- 粒子系统可视化
- 相机控制系统

## 技术栈

- C++20 标准
- OpenGL 3.3+
- GLFW（窗口和输入管理）
- GLAD（OpenGL 函数加载）
- GLM（数学运算）
- Eigen（线性代数运算）
- CMake（构建系统）

## 快速开始

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

### 运行

编译完成后，运行生成的可执行文件即可看到流体模拟效果。

## 项目结构

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

## 文档

详细的开发文档请参阅 [Doc](Doc) 目录：

- [开发指南](Doc/DevelopmentGuide.md) - 项目概述和整体架构
- [渲染管线](Doc/Pipeline.md) - 渲染核心逻辑
- [基本图元](Doc/Primitives.md) - 可渲染对象实现
- [着色器系统](Doc/Shader.md) - 着色器管理
- [核心组件](Doc/Core.md) - 基础数据结构
- [流体模拟](Doc/Fluid.md) - PBF算法实现

## 使用示例

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

## 许可证

本项目仅供学习和研究使用。