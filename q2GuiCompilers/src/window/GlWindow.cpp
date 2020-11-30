#include "GlWindow.h"

namespace Q2Compilers {

	EventCallbackFunc GlWindow::eventPushCallback;

	GlWindow::GlWindow(WindowProps& props, EventCallbackFunc pushFunc)
	{
		int init = glfwInit();
		ASSERT(init == GLFW_TRUE, "Failed to initialize glfw");

		glfwSetErrorCallback(ErrorCallback);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		ASSERT(_window, "Failed to initialize glfw window");

		glfwMakeContextCurrent(_window);

		int glad = gladLoadGL();
		ASSERT(glad, "Failed to initialize glad");

		eventPushCallback = pushFunc;

		//register input callbacks
		glfwSetMouseButtonCallback(_window, OnMouseButton);
		glfwSetScrollCallback(_window, OnMouseScroll);
		glfwSetCursorPosCallback(_window, OnMouseMoved);
		glfwSetKeyCallback(_window, OnKey);
	}

	GlWindow::~GlWindow()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	bool GlWindow::WindowShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	void GlWindow::OnUpdate()
	{
		glfwSwapBuffers(_window);
		glfwWaitEvents();
	}
}