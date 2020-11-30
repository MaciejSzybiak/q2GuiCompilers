#pragma once

#include "Core.h"

namespace Q2Compilers {

	struct WindowProps {
		std::string title;
		int width;
		int height;

		WindowProps(std::string title, int width, int height)
			: title(title), width(width), height(height) {}
	};

	class GlWindow
	{
	public:
		GlWindow(WindowProps& props, EventCallbackFunc pushFunc);
		~GlWindow();
		bool WindowShouldClose();
		void OnUpdate();
		GLFWwindow* GetGlfwWindow() {
			return _window;
		}
	private:
		inline static void ErrorCallback(int error, const char *description){
			LOG_ERROR("GLFW ERROR %d: %s", error, description);
		}
		inline static void OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
			if (action == GLFW_PRESS) {
				eventPushCallback(std::make_shared<MouseButtonPressedEvent>(button));
			}
			else
			{
				eventPushCallback(std::make_shared<MouseButtonReleasedEvent>(button));
			}
		}
		inline static void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset) {
			eventPushCallback(std::make_shared<MouseScrolledEvent>(xOffset, yOffset));
		}
		inline static void OnMouseMoved(GLFWwindow* window, double xPos, double yPos) {
			eventPushCallback(std::make_shared<MouseMovedEvent>(xPos, yPos));
		}
		inline static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
			case GLFW_REPEAT:
				eventPushCallback(std::make_shared<KeyPressedEvent>(key, action - 1));
				break;
			case GLFW_RELEASE:
				eventPushCallback(std::make_shared<KeyReleasedEvent>(key));
			}
		}

		GLFWwindow* _window;
		static EventCallbackFunc eventPushCallback;
	};
}

