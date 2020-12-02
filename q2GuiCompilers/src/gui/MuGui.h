#pragma once

#include "data/CompilerData.h"

namespace Q2Compilers
{
	struct MuGuiData
	{
		CData* data;
		std::vector<std::string> profileFiles;
		std::string profileName;
		std::string mapName;

		bool saveProfile = false;
		bool loadProfile = false;
		bool updateProfileList = false;
		bool compile = false;
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
		void DrawGamePanel(MuGuiData* data);
		void DrawQradPanel(MuGuiData* data);
		void DrawVisPanel(MuGuiData* data);
	};
}
