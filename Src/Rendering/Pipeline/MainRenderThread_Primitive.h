//
// Created by Jingren Bai on 25-8-3.
//

#ifndef LEARNOPENGL_MAINRENDERTHREAD_PRIMITIVE_H
#define LEARNOPENGL_MAINRENDERTHREAD_PRIMITIVE_H

#include <vector>
#include <string>
#include <mutex>
#include "Rendering/old_primitive_pipeline/Base/ObjectRender.h"

class MainRenderThread_Primitive {
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
	MainRenderThread_Primitive() = default;
	static MainRenderThread_Primitive& instance();
	static void init(int, int);
	static void add(std::unique_ptr<ObjectRender> object);
	void ensureRenderingStarted();

	~MainRenderThread_Primitive();

	void waitForExit();
};


#endif //LEARNOPENGL_MAINRENDERTHREAD_PRIMITIVE_H
