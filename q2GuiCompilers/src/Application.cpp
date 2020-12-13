#include "Core.h"
#include "Application.h"
#include "blargh.h"
#include "qbspi.h"
#include "qvisi.h"
#include <shlobj_core.h>

namespace Q2Compilers
{
	wchar_t dataPath[C_PATH_LENGTH] = { 0 };

	std::queue<std::shared_ptr<Event>> Application::_events;

	Application::Application(int argc, char **argv, std::string name)
	{
		Log::Init();

		SetDataPath();
		wcscpy_s(profilesDir, dataPath);
		wcscat_s(profilesDir, L"profiles\\");

		WindowProps props = WindowProps(name, 500, 720);

		_window = new GlWindow(props, Application::PushEvent);
		_renderer = new Renderer(_window->GetGlfwWindow());
		_gui = new MuGui(Renderer::GetTextWidth, props.width, props.height, props.title);
		_config = new Config();
		_compilerData = new CompilerData();
		_compiler = new	Compiler();

		//get settings from config
		strcpy_s(_guiData.mapName, _config->data.map_path);
		strcpy_s(_guiData.profileName, _config->data.profile_last);

		//get settings from args
		args = ArgParser::ParseArgs(argc, argv);
		if (strlen(args.mapName) > 0)
		{
			strcpy_s(_guiData.mapName, args.mapName);
		}
		if (strlen(args.profile) > 0)
		{
			strcpy_s(_guiData.profileName, args.profile);
		}
		_guiData.compile = _guiData.isCompiling = args.instant;

		//load profile
		if (strlen(_guiData.profileName) > 0)
		{
			LoadProfile(_guiData.profileName);
		}
	}

	Application::~Application()
	{
		SaveProfile("cached");
		strcpy(_config->data.map_path, _guiData.mapName);
		_config->WriteConfig();

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

			if (_guiData.isCompiling)
			{
				_guiData.isCompiling = _compiler->Update();
			}
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

	void Application::LoadProfile(const char* filename)
	{
		strcpy_s(_config->data.profile_last, filename);
		_compilerData->LoadFromFile(std::string(filename));
	}

	void Application::SaveProfile(const char *filename)
	{
		strcpy_s(_config->data.profile_last, filename);
		_compilerData->SaveFile(std::string(filename));
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
		for (const auto& entry : std::filesystem::directory_iterator(profilesDir))
		{
			const std::string& str = entry.path().string();
			if (!str.ends_with(PROFILE_EXTENSION_STR))
			{
				continue;
			}
			//strip path
			size_t start = str.find_last_of('\\') + 1;
			size_t end = str.find_last_of('.');

			vec.push_back(str.substr(start, end - start));
		}
	}

	void Application::Compile(const char *mapName, bool& isCompiling)
	{
		if (strlen(mapName) == 0)
		{
			LOG_WARNING("No map path specified.");
			isCompiling = false;
			return;
		}

		LOG_INFO("COMPILE! Map: %s", mapName);
		_guiData.isCompiling = _compiler->Compile(_guiData.data, mapName);
	}

	void Application::SetDataPath()
	{
		PWSTR path;
		HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
		wcscpy_s(dataPath, path);
		wcscat_s(dataPath, L"\\q2GuiCompilers\\");
		CreateDirectoryW(dataPath, NULL);
	}
}
