//
// Created by Jingren Bai on 25-8-3.
//

#include "MainRenderThread.h"
#include "utils/log.cpp"

MainRenderThread &MainRenderThread::instance() {
	static MainRenderThread instance;
	return instance;
}
MainRenderThread::~MainRenderThread() {
	stop();
}
void MainRenderThread::init(int width, int height){
//	instance().initWindow(width, height);
	instance().m_width = width;
	instance().m_height = height;
	instance().ensureRenderingStarted();
}
void MainRenderThread::ensureRenderingStarted(){
	std::lock_guard<std::mutex> lock(m_mutex);
	if(!m_renderingStarted && !renderPool.empty()){
		m_renderingStarted = true;
		m_renderThread = std::thread(&MainRenderThread::renderLoop, this);
		LOG_INFO << "started the main thread of render";
	}
}
void MainRenderThread::initWindow(int width, int height) {
	if(m_window) return;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	m_window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);
	if(m_window == nullptr){
		LOG_ERROR << "Failed to create GLFW window";
		glfwTerminate();
		/*
		 * 关闭所有由 GLFW 打开的窗口和上下文（Context）
		 * 释放 GLFW 内部分配的所有资源
		 * 重置 GLFW 的全局状态
		 *
		 * 不能在没有调用glfwInit()的情况下调用glfwTerminate()
		 * 在此之后，所有调用的glfw函数都无效
		 */
		exit(-1);
	}
	glfwMakeContextCurrent(m_window); // 设置窗口的上下文，确定后续的OpenGL函数调用所使用的OpenGL上下文
	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
		LOG_ERROR << "Failed to initialize GLAD";
		exit(-1);
	}
	/*
	 * 设置视口
	 * parameter:
	 * x, y, width, height
	 * x, y是视口左下角在窗口中的位置
	 * width, height是视口宽高
	 *
	 * 可以比实际的窗口小，这样窗口的内容就会缩放到视口大小内，并且将其他元素放置在视口之外
	 */
	glViewport(0, 0, width, height);

	// 注册回调函数，当窗口大小改变时，会调用这个回调函数(lambda)
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
		glViewport(0, 0, width, height);
	});
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
		instance().m_stopRequested = true;
	});
	glEnable(GL_DEPTH_TEST);
	LOG_INFO << "Window initialized";
}
void MainRenderThread::add(std::unique_ptr<ObjectRender> object) {
	instance().addObject(std::move(object));
}
void MainRenderThread::addObject(std::unique_ptr<ObjectRender> object) {
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		renderPool.push_back(std::move(object));
	}
	ensureRenderingStarted();
}
void MainRenderThread::renderLoop() {
	initWindow(m_width, m_height);
	glfwMakeContextCurrent(m_window);
	LOG_INFO << "Rendering loop started, m_stopRequested = " << m_stopRequested;
	while(!m_stopRequested){
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			processPendingObjects();
		}
		if(!m_objects.empty()){
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (auto &object: m_objects) {
				object->render();
			}
		}
		if(glfwWindowShouldClose(m_window)){
			LOG_INFO << "Window exited in normal";
			m_stopRequested = true;
		}
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		for(auto& object : m_objects){
			object->destroy();
		}
		m_objects.clear();
	}
}
void MainRenderThread::processPendingObjects() {
	m_objects.insert(m_objects.end(),
					 std::make_move_iterator(renderPool.begin()),
					 std::make_move_iterator(renderPool.end()));
	renderPool.clear();
	for(auto& object : m_objects){
		if(!object->isInitialized){
			object->init();
			object->isInitialized = true;
		}
	}
}
void MainRenderThread::stop() {
	m_stopRequested = true;
	LOG_INFO << "Stopping the main thread of render";
	if(m_renderThread.joinable()){
		m_renderThread.join();
	}
	if(m_window){
		glfwDestroyWindow(m_window);
		glfwTerminate();
		m_window = nullptr;
	}
}
void MainRenderThread::waitForExit() {
	if (m_renderThread.joinable()) {
		m_renderThread.join();
	}
}