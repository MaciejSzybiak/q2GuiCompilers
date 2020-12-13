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

		wchar_t profilesDir[C_PATH_LENGTH];

		void LoadProfile(const char *filename);
		void SaveProfile(const char *filename);

		void ProcessGuiData();
		void GetProfileNames(std::vector<std::string>& vec);
		void Compile(const char* mapName, bool& isCompiling);

		void SetDataPath();
	};
}

