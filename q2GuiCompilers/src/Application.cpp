#include "Core.h"
#include "Application.h"

namespace Q2Compilers
{
	std::queue<std::shared_ptr<Event>> Application::_events;

	Application::Application(int argc, char **argv, std::string name)
	{
		Log::Init();

		WindowProps props = WindowProps(name, 400, 720);

		_window = new GlWindow(props, Application::PushEvent);
		_renderer = new Renderer(_window->GetGlfwWindow());
		_gui = new MuGui(Renderer::GetTextWidth, props.width, props.height, props.title);
		_config = new Config();
		_compilerData = new CompilerData();

		//get settings from config
		_guiData.mapName = _config->GetCurrentData()->map_path;
		_guiData.profileName = _config->GetCurrentData()->profile_last;

		//get settings from args
		args = ArgParser::ParseArgs(argc, argv);
		if (args.mapName.size() > 0)
		{
			_guiData.mapName = args.mapName;
		}
		if (args.profile.size() > 0)
		{
			_guiData.profileName = args.profile;
		}
		_guiData.compile = _guiData.isCompiling = args.instant;

		//load profile
		if (_guiData.profileName.length() > 0 && _guiData.profileName.ends_with(".json"))
		{
			size_t dot = _guiData.profileName.find_last_of('.');
			_guiData.profileName = _guiData.profileName.substr(0, dot);
		}
		LoadProfile(_guiData.profileName);
	}

	Application::~Application()
	{
		_config->GetCurrentData()->map_path = _guiData.mapName;
		_config->WriteConfig();
		SaveProfile("cached");

		delete _gui;
		delete _renderer;
		delete _window;
		delete _compilerData;
	}

	void Application::Run()
	{
		GetProfileNames(_guiData.profileFiles);

		//main loop
		while (!_window->WindowShouldClose())
		{
			DispatchEvents();
			if (!_gui->MakeCommands(&_guiData))
			{
				break;
			}
			_renderer->ProcessCommands(_gui->GetContext(), mu_color(128, 128, 128, 255));
			_renderer->Finish();

			ProcessGuiData();

			_window->OnUpdate();
		}
	}

	void Application::PushEvent(std::shared_ptr<Event> event)
	{
		_events.push(event);
	}

	void Application::DispatchEvents()
	{
		while (!_events.empty())
		{
			std::shared_ptr<Event> e = _events.front();

			_gui->HandleEvent(e);

			_events.pop();
		}
	}

	void Application::LoadProfile(std::string filename)
	{
		filename += ".json";
		_config->GetCurrentData()->profile_last = filename;
		_compilerData->LoadFromFile(filename);
	}

	void Application::SaveProfile(std::string filename)
	{
		filename += ".json";
		_config->GetCurrentData()->profile_last = filename;
		_compilerData->SaveFile(filename);
	}

	void Application::ProcessGuiData()
	{
		_guiData.data = _compilerData->GetCurrentData();

		if (_guiData.saveProfile)
		{
			SaveProfile(_guiData.profileName);
			_guiData.saveProfile = false;
		}
		if (_guiData.loadProfile)
		{
			LoadProfile(_guiData.profileName);
			_guiData.loadProfile = false;
		}
		if (_guiData.updateProfileList)
		{
			GetProfileNames(_guiData.profileFiles);
			_guiData.updateProfileList = false;
		}
		if (_guiData.compile)
		{
			Compile(_guiData.mapName, _guiData.isCompiling);
			_guiData.compile = false;
		}
	}

	void Application::GetProfileNames(std::vector<std::string>& vec)
	{
		vec.clear();

		if (!std::filesystem::exists("profiles/"))
		{
			LOG_WARNING("Directory \"profiles\" doesn't exist");
			return;
		}
		for (const auto& entry : std::filesystem::directory_iterator("profiles/"))
		{
			const std::string& str = entry.path().string();
			if (!str.ends_with(".json"))
			{
				continue;
			}
			//strip path
			size_t start = str.find_last_of('/') + 1;
			size_t end = str.find_last_of('.');

			vec.push_back(str.substr(start, end - start));
		}
	}

	void Application::Compile(const std::string& mapName, bool& isCompiling)
	{
		if (mapName.length() == 0)
		{
			LOG_WARNING("No map path specified.");
			isCompiling = false;
			return;
		}

		LOG_INFO("COMPILE! Map: %s", mapName.c_str());
	}
}
