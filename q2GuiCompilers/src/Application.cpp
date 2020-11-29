#include "Application.h"
#include <iostream>

namespace Q2Compilers {

	std::queue<std::shared_ptr<Event>> Application::_events;

	Application::Application(std::string name)
	{
		WindowProps props = WindowProps(name, 1280, 720);
		
		_window = new GlWindow(props, Application::PushEvent);
	}

	Application::~Application()
	{
		delete _window;
	}

	void Application::Run()
	{
		while (!_window->WindowShouldClose()) {
			_window->OnUpdate();
			DispatchEvents();
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
			LOG_TRACE("Handling event: %s", e->ToString().c_str());

			switch (e->GetType())
			{
			case EventType::MouseButtonPressed:
			{
				auto event = std::dynamic_pointer_cast<MouseButtonPressedEvent>(e);
				break;
			}
			case EventType::MouseButtonReleased:
			{
				auto event = std::dynamic_pointer_cast<MouseButtonReleasedEvent>(e);
				break;
			}
			case EventType::MouseScrolled:
			{
				auto event = std::dynamic_pointer_cast<MouseScrolledEvent>(e);
				break;
			}
			case EventType::MouseMoved:
			{
				auto event = std::dynamic_pointer_cast<MouseMovedEvent>(e);
				break;
			}
			case EventType::KeyPressed:
			{
				auto event = std::dynamic_pointer_cast<KeyPressedEvent>(e);
				break;
			}
			case EventType::KeyReleased:
			{
				auto event = std::dynamic_pointer_cast<KeyReleasedEvent>(e);
				break;
			}
			default:
			{
				LOG_ERROR("Unhandled event type");
				__debugbreak();
				break;
			}
			}

			_events.pop();
		}
	}
}
