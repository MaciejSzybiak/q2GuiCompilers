#include "Core.h"

#include "Application.h"

namespace Q2Compilers {

	std::queue<std::shared_ptr<Event>> Application::_events;

	Application::Application(std::string name)
	{
		WindowProps props = WindowProps(name, 600, 800);
		
		_window = new GlWindow(props, Application::PushEvent);
		_renderer = new Renderer(_window->GetGlfwWindow());
		_gui = new MuGui(Renderer::GetTextWidth, props.width, props.height, props.title);
		_config = new Config();
		_compilerData = new CompilerData();

		std::string last = _config->GetCurrentData()->profile_last;

		if (last.length() > 0) {
			LoadProfile(last);
		}
	}

	Application::~Application()
	{
		SaveProfile("cached.json");

		delete _gui;
		delete _renderer;
		delete _window;
		delete _compilerData;
	}

	void Application::Run()
	{
		MuGuiData guiData;

		//main loop
		while (!_window->WindowShouldClose()) {
			DispatchEvents();
			if (!_gui->MakeCommands(&guiData)) {
				break;
			}
			_renderer->ProcessCommands(_gui->GetContext(), mu_color(128, 128, 128, 255));
			_renderer->Finish();

			ProcessGuiData(&guiData);

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

	void Application::LoadProfile(std::string filename)
	{
		_config->SetLastProfile(filename);
		_compilerData->LoadFromFile(filename);
	}

	void Application::SaveProfile(std::string filename) 
	{
		_config->SetLastProfile(filename);
		_compilerData->SaveFile(filename);
	}

	void Application::ProcessGuiData(MuGuiData* data)
	{
		data->data = _compilerData->GetCurrentData();

		if (data->saveProfile) {
			SaveProfile(data->profileName);
			data->saveProfile = false;
		}
		if (data->loadProfile) {
			LoadProfile(data->profileName);
			data->loadProfile = false;
		}
		if (data->updateProfileList) {
			data->updateProfileList = false;
		}
		if (data->compile) {
			Compile(data->mapName);
			data->compile = false;
		}
	}

	void Application::GetProfileNames(std::vector<std::string>* vec)
	{
		vec->clear();

		if (!std::filesystem::exists("profiles/")) {
			LOG_WARNING("Directory \"profiles\" doesn't exist");
			return;
		}

		for (const auto& entry : std::filesystem::directory_iterator("profiles/")) {
			std::string str = entry.path().string();
			size_t start = str.find_last_of('/');
			vec->push_back(str.substr(start + 1));
		}
	}

	void Application::Compile(std::string mapName)
	{
		LOG_INFO("COMPILE! Map: %s", mapName.c_str());
	}
}
