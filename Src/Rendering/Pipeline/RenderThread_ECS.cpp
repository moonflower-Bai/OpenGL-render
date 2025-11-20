//
// Created by Jingren Bai on 25-11-12.
//

#include "RenderThread_ECS.h"

// Define static members declared in header
std::atomic<bool> RenderThread_ECS::s_glReady{false};
std::condition_variable RenderThread_ECS::s_cv;
std::mutex RenderThread_ECS::s_cvMutex;

RenderThread_ECS& RenderThread_ECS::instance() {
	static RenderThread_ECS instance;
	return instance;
}

RenderThread_ECS::~RenderThread_ECS() {
	stop();
}

void RenderThread_ECS::init(int width, int height) {
	LOG_INFO << "Initializing ECS Render Thread with window size: "
			  << width << "x" << height;
	instance().m_width = width;
	instance().m_height = height;
	instance().ensureRenderingStarted();
}

void RenderThread_ECS::ensureRenderingStarted() {
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!m_renderingStarted && !m_entityPool.empty()) {
		m_renderingStarted = true;
		m_renderThread = std::thread(&RenderThread_ECS::renderLoop, this);
		LOG_INFO << "Started ECS main render thread.";
	}
}

void RenderThread_ECS::initWindow(int width, int height) {
	if (m_window) return;

	if (!glfwInit()) {
		LOG_ERROR << "GLFW initialization failed.";
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_window = glfwCreateWindow(width, height, "ECS Renderer", nullptr, nullptr);
	if (!m_window) {
		LOG_ERROR << "Failed to create GLFW window.";
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR << "Failed to initialize GLAD.";
		exit(-1);
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow*, int w, int h) {
		glViewport(0, 0, w, h);
	});
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow*) {
		instance().m_stopRequested = true;
	});

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glfwSwapInterval(0);
	LOG_INFO << "OpenGL window initialized (" << width << "x" << height << ")";
}

void RenderThread_ECS::addEntity(std::shared_ptr<Entity> entity) {
	instance().addEntityInternal(std::move(entity));
}

void RenderThread_ECS::addEntityInternal(std::shared_ptr<Entity> entity) {
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_entityPool.push_back(std::move(entity));
	}
	ensureRenderingStarted();
}

void RenderThread_ECS::renderLoop() {
	initWindow(m_width, m_height);
	glfwMakeContextCurrent(m_window);


	LOG_INFO << "[RenderThread_ECS] Render loop started.";

	{
		std::lock_guard<std::mutex> lk(s_cvMutex);
		s_glReady.store(true);
	}
	s_cv.notify_all();

	while (!m_stopRequested) {
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			processPendingEntities();
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

		// 更新所有实体（即组件的 onUpdate）
		for (auto& entity : m_entities) {
			entity->update(0.016f);
		}

		glfwSwapBuffers(m_window);
		glfwPollEvents();
//		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	LOG_INFO << "Render loop exited, cleaning up.";

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_entities.clear();
		m_entityPool.clear();
	}

	glfwDestroyWindow(m_window);
	glfwTerminate();
	m_window = nullptr;
}

void RenderThread_ECS::processPendingEntities() {
	m_entities.insert(m_entities.end(),
					  std::make_move_iterator(m_entityPool.begin()),
					  std::make_move_iterator(m_entityPool.end()));
	m_entityPool.clear();
}

void RenderThread_ECS::stop() {
	m_stopRequested = true;
	LOG_INFO << "Stopping render thread...";
	if (m_renderThread.joinable())
		m_renderThread.join();
}

void RenderThread_ECS::waitForExit() {
	if (m_renderThread.joinable())
		m_renderThread.join();
}

bool RenderThread_ECS::isGLReady() {
	return s_glReady.load();
}
