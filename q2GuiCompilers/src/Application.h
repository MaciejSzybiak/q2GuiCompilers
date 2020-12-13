#pragma once
#include "window/GlWindow.h"
#include "render/Renderer.h"
#include "gui/MuGui.h"
#include "data/CompilerData.h"
#include "data/Config.h"
#include "ArgParser.h"
#include "compiler/Compiler.h"

namespace Q2Compilers
{
	class Application
	{
	public:
		Application(int argc, char** argv, std::string name);
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
		MuGuiData _guiData;
		Compiler* _compiler;
		ArgOptions args;

		static std::queue<std::shared_ptr<Event>> _events;

		void LoadProfile(const char *filename);
		void SaveProfile(const char *filename);

		void ProcessGuiData();
		void GetProfileNames(std::vector<std::string>& vec);
		void Compile(const std::string& mapName, bool& isCompiling);

		void SetDataPath();
	};
}

