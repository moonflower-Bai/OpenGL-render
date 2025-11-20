# 核心组件 (Core) 模块文档

## 1. 模块概述

核心组件模块包含渲染系统的基础构建块，如顶点定义、纹理管理等。这些组件为上层渲染模块提供基础数据结构和功能支持。

## 2. 核心类

### 2.1 Vertex 结构体

Vertex 结构体定义了顶点数据的格式，包含顶点的位置、颜色和纹理坐标信息。

#### 结构体定义

```cpp
struct alignas(16) Vertex {
    Vertex() = default;
    Vertex(Eigen::Vector4f color, Eigen::Vector4f position, Eigen::Vector2f texCoord){
        this->color = color;
        this->position = position;
        this->texCoord = texCoord;
    }
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Eigen::Vector4f color;
    Eigen::Vector4f position;
    Eigen::Vector2f texCoord;

    static Vertex particlesToVertex(Particle particle){
        return {Eigen::Vector4f(1-particle.density, 1-particle.density, 1-particle.density, 1.0f),
                Eigen::Vector4f(particle.pos.x(), particle.pos.y(), -particle.pos.z(), 10.0f),
                Eigen::Vector2f(0, 0)};
    }
};
```

#### 成员变量说明

- [color](file:///D:/Code/LearnOpenGL/Rendering/Core/Vertex.h#L17-L17): 顶点颜色，使用 RGBA 格式
- [position](file:///D:/Code/LearnOpenGL/Rendering/Core/Vertex.h#L18-L18): 顶点位置，使用齐次坐标
- [texCoord](file:///D:/Code/LearnOpenGL/Rendering/Core/Vertex.h#L19-L19): 纹理坐标

#### 核心方法说明

##### Vertex(Eigen::Vector4f color, Eigen::Vector4f position, Eigen::Vector2f texCoord)
构造函数，创建一个顶点对象。

参数：
- color: 顶点颜色
- position: 顶点位置
- texCoord: 纹理坐标

##### particlesToVertex(Particle particle)
静态方法，将粒子对象转换为顶点对象。

参数：
- particle: 粒子对象

返回值：
- Vertex: 对应的顶点对象

该方法将粒子的密度信息映射为颜色，位置信息映射为顶点位置。

### 2.2 Texture 类

Texture 类封装了 OpenGL 纹理对象的加载和管理功能。

#### 类定义

```cpp
class Texture {
private:
    unsigned int texture;
    std::string texturePath;
    void initTexture();
    
public:
    Texture(std::string path);
    Texture(Texture &&texture) noexcept ;
    Texture(const Texture &texture) = delete;
    Texture& operator=(Texture &&texture) noexcept ;
    Texture& operator=(const Texture &texture) = delete;
    bool empty() const;
    Texture();
    ~Texture();
    void bind();
};
```

#### 核心属性说明

- [texture](file:///D:/Code/LearnOpenGL/Rendering/Core/Texture.h#L13-L13): OpenGL 纹理对象 ID
- [texturePath](file:///D:/Code/LearnOpenGL/Rendering/Core/Texture.h#L14-L14): 纹理文件路径

#### 核心方法说明

##### Texture(std::string path)
构造函数，从指定路径加载纹理。

参数：
- path: 纹理文件路径（相对于项目根目录）

工作流程：
1. 获取项目根目录路径
2. 拼接完整纹理文件路径
3. 调用 [initTexture()](file:///D:/Code/LearnOpenGL/Rendering/Core/Texture.h#L15-L15) 初始化纹理

##### initTexture()
初始化 OpenGL 纹理对象。

工作流程：
1. 生成纹理对象
2. 绑定纹理对象
3. 设置纹理参数（环绕方式、过滤方式）
4. 使用 stb_image 库加载纹理图像数据
5. 将图像数据上传到 GPU
6. 生成 Mipmap

##### bind()
绑定纹理对象，使其成为当前活动的纹理。

##### empty() const
检查纹理对象是否为空。

返回值：
- bool: 纹理对象为空返回 true，否则返回 false

##### ~Texture()
析构函数，释放 OpenGL 纹理资源。

## 3. 使用示例

```cpp
// 创建顶点对象
Vertex vertex(
    Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f),  // 红色
    Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f),  // 原点
    Eigen::Vector2f(0.0f, 0.0f)               // 纹理坐标(0,0)
);

// 加载纹理
Texture texture("/Texture/wall.jpg");
texture.bind();  // 绑定纹理
```