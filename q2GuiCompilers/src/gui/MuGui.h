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

#include "data/CompilerData.h"

namespace Q2Compilers
{
	struct MuGuiData
	{
		CData* data = nullptr;
		std::vector<std::string> profileFiles;
		//std::string profileName;
		char profileName[C_PATH_LENGTH] = { 0 };
		//std::string mapName;
		char mapName[C_PATH_LENGTH] = { 0 };

		bool saveProfile = false;
		bool loadProfile = false;
		bool updateProfileList = false;
		bool compile = false;

		bool isCompiling = false;
	};

	class MuGui
	{
	public:
		MuGui(TextWidthFunc widthFunc, int xSize, int ySize, std::string name);
		void HandleEvent(std::shared_ptr<Event> e);
		bool MakeCommands(MuGuiData* data);
		mu_Context* GetContext() const
		{
			return _context;
		}
	private:
		mu_Context* _context;
		static TextWidthFunc _widthFunc;
		int _width;
		int _height;
		std::string _name;

		static int TextWidth(mu_Font font, const char* text, int len)
		{
			if (len == -1)
			{
				len = (int)strlen(text);
			}
			return _widthFunc(text, len);
		}
		static int TextHeight(mu_Font font)
		{
			return 18;
		}

		void DrawProfilePopup(bool isLoadPopup, MuGuiData* data, int windowWidth);
		void DrawProfilesPanel(MuGuiData* data, mu_Container* window);
		void DrawGamePanel(MuGuiData* data);
		void DrawQbspPanel(MuGuiData* data);
		void DrawQradPanel(MuGuiData* data);
		void DrawVisPanel(MuGuiData* data);
		void DrawConsolePanel();
		void DrawMapPanel(MuGuiData* data);

		void DrawSliderWithLabel(const char* label, int* value, float low, float high);
		void DrawSliderWithLabel(const char* label, float* value, float low, float high);
		int DrawIntSlider(int* value, float low, float high);

		bool TryGetPathFromFileDialog(const char* filter, char *out);
		bool TryGetPathFromFolderDialog(char* out);
	};
}
