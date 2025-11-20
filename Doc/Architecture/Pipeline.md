# 渲染管线 (Pipeline) 模块文档

## 1. 模块概述

渲染管线模块负责管理 OpenGL 渲染过程的核心逻辑，包括窗口初始化、渲染循环、多线程渲染管理等功能。该模块是整个渲染系统的核心，协调各个渲染对象的初始化、更新和渲染。

## 2. 核心类

### 2.1 MainRenderThread

MainRenderThread 是渲染管线的核心管理类，采用单例模式设计，负责：

- 窗口初始化和 OpenGL 上下文管理
- 多线程渲染循环控制
- 渲染对象池管理
- 渲染资源的统一管理与释放

#### 类定义

```cpp
class MainRenderThread {
private: // 状态
    GLFWwindow* m_window = nullptr;
    int m_width, m_height;
    std::mutex m_mutex;
    bool m_renderingStarted;
    bool m_stopRequested = false;
    std::thread m_renderThread;
    std::vector<std::unique_ptr<ObjectRender> > renderPool;
    std::vector<std::unique_ptr<ObjectRender> > m_objects;
    
private:
    void initWindow(int, int);
    void stop();
    void renderLoop();
    void processPendingObjects();
    void addObject(std::unique_ptr<ObjectRender> object);
    
public:
    MainRenderThread() = default;
    static MainRenderThread& instance();
    static void init(int, int);
    static void add(std::unique_ptr<ObjectRender> object);
    void ensureRenderingStarted();
    ~MainRenderThread();
    void waitForExit();
};
```

#### 核心方法说明

##### instance()
获取 MainRenderThread 单例实例。

##### init(int width, int height)
初始化渲染管线，设置窗口尺寸。

参数：
- width: 窗口宽度
- height: 窗口高度

##### add(std::unique_ptr<ObjectRender> object)
添加渲染对象到渲染池中。

参数：
- object: 需要渲染的对象指针

##### waitForExit()
等待渲染线程结束，阻塞当前线程直到渲染完成。

#### 工作流程

1. 通过 [init()](file:///D:/Code/LearnOpenGL/Rendering/Pipeline/MainRenderThread.h#L29-L29) 方法初始化渲染管线，设置窗口尺寸
2. 通过 [add()](file:///D:/Code/LearnOpenGL/Rendering/Pipeline/MainRenderThread.h#L34-L34) 方法添加渲染对象到渲染池
3. 当渲染池不为空且渲染未开始时，自动启动渲染线程
4. 渲染线程中：
   - 初始化 OpenGL 窗口和上下文
   - 处理待渲染对象（从渲染池移动到活动对象列表）
   - 执行渲染循环，依次调用每个对象的 [render()](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L28-L28) 方法
   - 处理窗口事件和帧同步
5. 通过 [waitForExit()](file:///D:/Code/LearnOpenGL/Rendering/Pipeline/MainRenderThread.h#L41-L41) 方法等待渲染结束

#### 多线程安全

该类通过互斥锁 ([std::mutex](file:///D:/ProgramFiles/Tools/vcpkg/installed/x64-windows/share/eigen3/Eigen/src/Core/util/NonMPL2.h#L115-L115)) 保证线程安全，确保在多线程环境下对渲染对象池和活动对象列表的访问是安全的。

#### 生命周期管理

MainRenderThread 负责管理所有渲染对象的生命周期，包括：
- 对象的初始化（调用 [init()](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L26-L26) 方法）
- 对象的渲染（调用 [render()](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L28-L28) 方法）
- 对象资源的释放（调用 [destroy()](file:///D:/Code/LearnOpenGL/Rendering/Primitives/ObjectRender.h#L29-L29) 方法）