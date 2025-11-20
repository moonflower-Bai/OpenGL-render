//
// Created by Jingren Bai on 25-11-12.
//

#ifndef LEARNOPENGL_RENDERTHREAD_ECS_H
#define LEARNOPENGL_RENDERTHREAD_ECS_H

#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/log.cpp"
#include "Rendering/Primitives/Entity/Entity.h"

class Entity;

class RenderThread_ECS {
private:
	GLFWwindow* m_window = nullptr;
	int m_width = 800, m_height = 600;
	std::mutex m_mutex;

	bool m_renderingStarted = false;
	bool m_stopRequested = false;

	// 用于确保OpenGL上下文初始化完成
	static std::atomic<bool> s_glReady;
	static std::condition_variable s_cv;
	static std::mutex s_cvMutex;

	std::thread m_renderThread;
	std::vector<std::shared_ptr<Entity>> m_entityPool;   // 临时待加入
	std::vector<std::shared_ptr<Entity>> m_entities;     // 活跃实体列表

private:
	void initWindow(int width, int height);
	void stop();
	void renderLoop();
	void processPendingEntities();

public:
	RenderThread_ECS() = default;
	~RenderThread_ECS();

	static RenderThread_ECS& instance();

	// 初始化窗口并启动渲染循环
	static void init(int width, int height);

	// 添加实体（线程安全）
	static void addEntity(std::shared_ptr<Entity> entity);

	// 内部版本（有锁）
	void addEntityInternal(std::shared_ptr<Entity> entity);

	void ensureRenderingStarted();
	void waitForExit();

	static bool isGLReady();
};


#endif //LEARNOPENGL_RENDERTHREAD_ECS_H
