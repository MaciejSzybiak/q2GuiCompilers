#pragma once
#include "window/GlWindow.h"
#include "render/Renderer.h"
#include "gui/MuGui.h"
#include "data/CompilerData.h"
#include "data/Config.h"

namespace Q2Compilers
{
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
		MuGui* _gui;
		CompilerData* _compilerData;
		Config* _config;
		static std::queue<std::shared_ptr<Event>> _events;

		void LoadProfile(std::string filename);
		void SaveProfile(std::string filename);

		void ProcessGuiData(MuGuiData* data);
		void GetProfileNames(std::vector<std::string>& vec);
		void Compile(const std::string& mapName);
	};
}

