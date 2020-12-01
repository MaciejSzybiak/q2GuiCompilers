#include "Core.h"
#include "GlWindow.h"

namespace Q2Compilers
{
	int GlWindow::_width;
	int GlWindow::_height;
	EventCallbackFunc GlWindow::eventPushCallback;

	GlWindow::GlWindow(const WindowProps& props, EventCallbackFunc pushFunc)
	{
		int init = glfwInit();
		ASSERT(init == GLFW_TRUE, "Failed to initialize glfw");

		glfwSetErrorCallback(ErrorCallback);

		_width = props.width;
		_height = props.height;

		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

		_window = glfwCreateWindow(_width, _height, props.title.c_str(), NULL, NULL);
		ASSERT(_window, "Failed to initialize glfw window");

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int w = (mode->width / 2) - (_width / 2);
		int h = (mode->height / 2) - (_height / 2);
		glfwSetWindowPos(_window, w, h);

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

	bool GlWindow::WindowShouldClose() const
	{
		return glfwWindowShouldClose(_window);
	}

	void GlWindow::OnUpdate() const
	{
		glfwSwapBuffers(_window);
		glfwWaitEvents();
	}
}