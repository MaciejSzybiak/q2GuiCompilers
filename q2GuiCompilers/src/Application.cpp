#include "Application.h"
#include <iostream>

namespace Q2Compilers {

	std::queue<std::shared_ptr<Event>> Application::_events;

	Application::Application(std::string name)
	{
		WindowProps props = WindowProps(name, 600, 800);
		
		_window = new GlWindow(props, Application::PushEvent);
		_renderer = new Renderer(_window->GetGlfwWindow());
		_gui = new MuGui(Renderer::GetTextWidth, props.width, props.height, props.title);
	}

	Application::~Application()
	{
		delete _gui;
		delete _renderer;
		delete _window;
	}

	void Application::Run()
	{
		//main loop
		while (!_window->WindowShouldClose()) {
			DispatchEvents();
			if (!_gui->MakeCommands()) {
				break;
			}
			_renderer->ProcessCommands(_gui->GetContext(), mu_color(128, 128, 128, 255));
			_renderer->Finish();
			_window->OnUpdate();
		}
	}

	void Application::PushEvent(std::shared_ptr<Event> event)
	{
		_events.push(event);
	}

	void Application::DispatchEvents()
	{
		while (!_events.empty()) {
			std::shared_ptr<Event> e = _events.front();
			
			_gui->HandleEvent(e);

			_events.pop();
		}
	}
}
