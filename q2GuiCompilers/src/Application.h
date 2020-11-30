#pragma once

#include "window/GlWindow.h"
#include "render/Renderer.h"

namespace Q2Compilers {

	class Application
	{
	public:
		Application(std::string name);
		~Application();
		void Run();
		static void PushEvent(std::shared_ptr<Event> event);
		void DispatchEvents();
	private:
		GlWindow* _window;
		Renderer* _renderer;
		static std::queue<std::shared_ptr<Event>> _events;
	};
}

