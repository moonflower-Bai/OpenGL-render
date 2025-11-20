# 着色器 (Shader) 模块文档

## 1. 模块概述

着色器模块负责 OpenGL 着色器程序的加载、编译、链接和管理。该模块封装了着色器的完整生命周期，包括从文件读取着色器源码、编译着色器、链接程序对象以及设置着色器参数等功能。

## 2. 核心类

### 2.1 Shader 类

Shader 类封装了 OpenGL 着色器程序的管理功能，包括顶点着色器和片段着色器的编译链接，以及着色器参数的设置。

#### 类定义

```cpp
class Shader {
private:
    unsigned int programID;
    void checkCompileErrors(unsigned int shader);
    std::string vertexPath;
    std::string fragmentPath;
    
public:
    Shader() = default;
    Shader(const Shader &);
    Shader(Shader &&) noexcept;
    Shader(std::string vertexPath, std::string fragmentPath);
    Shader& operator=(Shader);
    ~Shader();
    void use();
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, float x, float y, float z);
    void setVec3(const std::string &name, Eigen::Vector3f value);
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setVec4(const std::string &name, Eigen::Vector4f value);
    void setMat4(const std::string &name, Eigen::Matrix4f value);
    void setMat4(const std::string &name, glm::mat4 value);
    std::string getVertexPath();
    std::string getFragmentPath();
    unsigned int getProgramID();
};
```

#### 核心属性说明

- [programID](file:///D:/Code/LearnOpenGL/Shader/Shader.h#L12-L12): OpenGL 着色器程序对象的 ID
- [vertexPath](file:///D:/Code/LearnOpenGL/Shader/Shader.h#L15-L15): 顶点着色器文件路径
- [fragmentPath](file:///D:/Code/LearnOpenGL/Shader/Shader.h#L16-L16): 片段着色器文件路径

#### 核心方法说明

##### Shader(std::string vertexPath, std::string fragmentPath)
构造函数，从指定路径加载并编译着色器。

参数：
- vertexPath: 顶点着色器文件路径
- fragmentPath: 片段着色器文件路径

工作流程：
1. 使用 [ReadShader](file:///D:/Code/LearnOpenGL/utils/ReadShader.h#L8-L13) 工具类读取着色器源码
2. 创建顶点着色器对象并编译
3. 创建片段着色器对象并编译
4. 创建着色器程序对象并链接顶点和片段着色器
5. 检查编译和链接错误
6. 删除独立的着色器对象

##### use()
激活当前着色器程序，使其成为当前渲染上下文的活动程序。

##### setInt(const std::string &name, int value)
设置着色器中的整型 uniform 变量。

参数：
- name: uniform 变量名称
- value: 整型值

##### setFloat(const std::string &name, float value)
设置着色器中的浮点型 uniform 变量。

参数：
- name: uniform 变量名称
- value: 浮点型值

##### setVec3(const std::string &name, float x, float y, float z)
设置着色器中的三维向量 uniform 变量。

参数：
- name: uniform 变量名称
- x, y, z: 向量的三个分量

##### setVec3(const std::string &name, Eigen::Vector3f value)
设置着色器中的三维向量 uniform 变量。

参数：
- name: uniform 变量名称
- value: Eigen 三维向量

##### setVec4(const std::string &name, float x, float y, float z, float w)
设置着色器中的四维向量 uniform 变量。

参数：
- name: uniform 变量名称
- x, y, z, w: 向量的四个分量

##### setVec4(const std::string &name, Eigen::Vector4f value)
设置着色器中的四维向量 uniform 变量。

参数：
- name: uniform 变量名称
- value: Eigen 四维向量

##### setMat4(const std::string &name, Eigen::Matrix4f value)
设置着色器中的 4x4 矩阵 uniform 变量。

参数：
- name: uniform 变量名称
- value: Eigen 4x4 矩阵

##### setMat4(const std::string &name, glm::mat4 value)
设置着色器中的 4x4 矩阵 uniform 变量。

参数：
- name: uniform 变量名称
- value: GLM 4x4 矩阵

##### getProgramID()
获取着色器程序对象的 ID。

返回值：
- unsigned int: 程序对象 ID

### 2.2 ReadShader 工具类

ReadShader 是一个辅助类，用于从文件中读取着色器源码。

#### 类定义

```cpp
class ReadShader {
private:
    char* shaderSource;
    std::string shaderPath;
public:
    explicit ReadShader(std::string path);
    ~ReadShader();
    char* getShader();
};
```

#### 核心方法说明

##### ReadShader(std::string path)
构造函数，从指定路径读取着色器源码。

参数：
- path: 着色器文件路径

##### getShader()
获取着色器源码。

返回值：
- char*: 着色器源码字符串指针

## 3. 着色器文件

### 3.1 顶点着色器示例 (Triangle.vert)

```glsl
#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * aPos;
    ourColor = aColor;
    TexCoord = aTexCoord;
}
```

### 3.2 片段着色器示例 (Triangle.frag)

```glsl
#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * ourColor;
}
```

## 4. 使用示例

```cpp
// 创建着色器程序
Shader shader("Shader/Triangle.vert", "Shader/Triangle.frag");

// 激活着色器
shader.use();

// 设置 uniform 变量
shader.setVec4("ourColor", 1.0f, 0.5f, 0.2f, 1.0f);
shader.setMat4("model", modelMatrix);
```