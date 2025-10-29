//
// Created by Jingren Bai on 25-8-3.
//

#ifndef LEARNOPENGL_MAINRENDERTHREAD_H
#define LEARNOPENGL_MAINRENDERTHREAD_H

#include <vector>
#include <string>
#include <mutex>
#include "Rendering/Primitives/Base/ObjectRender.h"

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


#endif //LEARNOPENGL_MAINRENDERTHREAD_H
