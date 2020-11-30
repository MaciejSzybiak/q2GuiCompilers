#pragma once

#include "Core.h"
#include "microui.h"

#define WINDOW_BORDER 2

namespace Q2Compilers {
	
	class MuGui
	{
	public:
		MuGui(TextWidthFunc widthFunc, int xSize, int ySize, std::string name);
		void HandleEvent(std::shared_ptr<Event> e);
		bool MakeCommands();
		mu_Context* GetContext() {
			return _context;
		}
	private:
		mu_Context* _context;
		static TextWidthFunc _widthFunc;
		int _width;
		int _height;
		std::string _name;

		static int TextWidth(mu_Font font, const char* text, int len) {
			if (len == -1) {
				len = (int)strlen(text);
			}
			return _widthFunc(text, len);
		}
		static int TextHeight(mu_Font font) {
			return 18;
		}
	};
}
