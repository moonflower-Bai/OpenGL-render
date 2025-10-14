# 基本图元 (Primitives) 模块文档

## 1. 模块概述

基本图元模块负责实现各种可渲染对象的具体实现，包括三角形、点等基本图形的渲染逻辑。该模块继承自 ObjectRender 基类，为不同类型的图形提供具体的初始化、更新和渲染实现。

## 2. 核心类

### 2.1 ObjectRender 基类

ObjectRender 是所有可渲染对象的基类，定义了渲染对象的基本接口和通用属性。

#### 类定义

```cpp
class ObjectRender {
protected:
    std::string m_texturePath;
    std::vector<Vertex> vertices;
    unsigned int programID;
    unsigned int VAO, VBO, EBO;
    Eigen::Vector4f position;
    Camera m_camera;
    
public:
    bool isInitialized = false;
    virtual ~ObjectRender() = default;
    ObjectRender() = default;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void destroy();
    void setCamera(Camera camera);
    void setPosition(Eigen::Vector4f position);
};
```

#### 核心属性说明

- [m_texturePath](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L17-L17): 纹理文件路径
- [vertices](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L18-L18): 顶点数据列表
- [programID](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L19-L19): 着色器程序ID
- [VAO, VBO, EBO](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L20-L20): OpenGL 缓冲对象
- [position](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L22-L22): 对象在世界坐标系中的位置
- [m_camera](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L23-L23): 相机对象
- [isInitialized](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L25-L25): 初始化状态标志

#### 核心方法说明

##### init()
纯虚函数，由子类实现，负责初始化渲染对象的 OpenGL 资源。

##### update()
纯虚函数，由子类实现，负责更新对象状态。

##### render()
纯虚函数，由子类实现，负责执行实际的渲染操作。

##### destroy()
销毁 OpenGL 资源，包括着色器程序和缓冲对象。

##### setCamera(Camera camera)
设置相机对象。

参数：
- camera: 相机实例

##### setPosition(Eigen::Vector4f position)
设置对象在世界坐标系中的位置。

参数：
- position: 位置向量

### 2.2 TriangleRendering 类

TriangleRendering 实现三角形（包括三角形列表）的渲染功能，支持纹理映射。

#### 类定义

```cpp
class TriangleRendering : public ObjectRender {
private:
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
    Texture m_texture;
    Shader m_shader;
    
public:
    explicit TriangleRendering(const std::vector<Vertex> vertices, std::string vertexShaderPath, std::string fragmentShaderPath, std::string texturePath);
    TriangleRendering();
    void setVertices(std::vector<Vertex> vertices);
    std::vector<Vertex> getVertices();
    void setVertexShaderPath(std::string vertexShaderPath);
    void setFragmentShaderPath(std::string fragmentShaderPath);
    void setTexturePath(std::string texturePath);
    void init() override;
    void update() override { /* empty */}
    void render() override;
    void destroy() override;
    Shader* getShader();
};
```

#### 核心方法说明

##### TriangleRendering(const std::vector<Vertex> vertices, std::string vertexShaderPath, std::string fragmentShaderPath, std::string texturePath)
构造函数，初始化三角形渲染对象。

参数：
- vertices: 顶点数据列表
- vertexShaderPath: 顶点着色器文件路径
- fragmentShaderPath: 片段着色器文件路径
- texturePath: 纹理文件路径

##### init() override
初始化三角形渲染对象，包括：
1. 编译链接着色器程序
2. 创建和配置顶点数组对象 (VAO) 和顶点缓冲对象 (VBO)
3. 加载和绑定纹理

##### render() override
执行三角形渲染，包括：
1. 激活着色器程序
2. 绑定纹理
3. 设置变换矩阵（投影、视图、模型）
4. 执行绘制命令

### 2.3 PointRender 类

PointRender 实现点图元的渲染功能，主要用于粒子系统的可视化。

#### 类定义

```cpp
class PointRender : public ObjectRender {
private:
    Simulator simulator;
    Shader m_shader;
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
    
public:
    PointRender(int particleNums, std::string vertexShaderPath, std::string fragmentShaderPath);
    void setVertices(std::vector<Vertex> vertices);
    std::vector<Vertex> getVertices();
    void init() override;
    void render() override;
    void update() override;
    void destroy() override;
};
```

#### 核心方法说明

##### PointRender(int particleNums, std::string vertexShaderPath, std::string fragmentShaderPath)
构造函数，初始化点渲染对象。

参数：
- particleNums: 粒子数量
- vertexShaderPath: 顶点着色器文件路径
- fragmentShaderPath: 片段着色器文件路径

##### init() override
初始化点渲染对象，包括：
1. 初始化流体模拟器
2. 创建和配置顶点数组对象 (VAO) 和顶点缓冲对象 (VBO)
3. 编译链接着色器程序

##### update() override
更新粒子系统状态：
1. 运行 PBF 流体模拟算法
2. 更新顶点缓冲区数据

##### render() override
执行点渲染，包括：
1. 更新粒子状态
2. 激活着色器程序
3. 设置变换矩阵
4. 执行点图元绘制命令