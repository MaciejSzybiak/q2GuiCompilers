/*
	This file is a part of q2GuiCompilers.
	Copyright (C) 2020  Maciej Szybiak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
		glfwSetCharCallback(_window, OnTextInput);
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
		glfwPollEvents();
	}
}