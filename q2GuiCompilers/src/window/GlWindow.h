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

#pragma once

namespace Q2Compilers
{
	struct WindowProps
	{
		std::string title;
		int width;
		int height;

		WindowProps(std::string title, int width, int height)
			: title(title), width(width), height(height)
		{}
	};

	class GlWindow
	{
	public:
		GlWindow(const WindowProps& props, EventCallbackFunc pushFunc);
		~GlWindow();
		bool WindowShouldClose() const;
		void OnUpdate() const;
		GLFWwindow* GetGlfwWindow()
		{
			return _window;
		}
	private:
		static void ErrorCallback(int error, const char* description)
		{
			LOG_ERROR("GLFW ERROR %d: %s", error, description);
		}

		static void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);

			if (action == GLFW_PRESS)
			{
				eventPushCallback(std::make_shared<MouseButtonPressedEvent>(button, (int)(x), (int)(y)));
			}
			else
			{
				eventPushCallback(std::make_shared<MouseButtonReleasedEvent>(button, (int)(x), (int)(y)));
			}
		}

		static void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
		{
			eventPushCallback(std::make_shared<MouseScrolledEvent>((float)(xOffset), (float)(yOffset)));
		}

		static void OnMouseMoved(GLFWwindow* window, double xPos, double yPos)
		{
			eventPushCallback(std::make_shared<MouseMovedEvent>((int)(xPos), (int)(yPos)));
		}

		static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (action)
			{
			case GLFW_PRESS:
			case GLFW_REPEAT:
			{
				eventPushCallback(std::make_shared<KeyPressedEvent>(key, action - 1));
				break;
			}
			case GLFW_RELEASE:
				eventPushCallback(std::make_shared<KeyReleasedEvent>(key));
			}
		}

		static void OnTextInput(GLFWwindow* window, unsigned int key)
		{
			eventPushCallback(std::make_shared<TextInputEvent>(key));
		}

		GLFWwindow* _window;
		static int _width;
		static int _height;
		static EventCallbackFunc eventPushCallback;
	};


}

