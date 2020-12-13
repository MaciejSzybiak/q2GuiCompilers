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

#include "Core.h"

namespace Q2Compilers
{
	std::stringstream Log::buffer;

	void Log::Init()
	{
		std::cout.rdbuf(buffer.rdbuf());

		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	void Log::LogMessage(LogType type, const char* string)
	{
		switch (type)
		{
		case Q2Compilers::LogType::warning:
			std::cout << "> WARNING: ";
			break;
		case Q2Compilers::LogType::error:
			std::cout << "> ERROR: ";
			break;
		case Q2Compilers::LogType::assert:
			std::cout << "> ASSERTION FAILED: ";
			break;
		case Q2Compilers::LogType::trace:
		case Q2Compilers::LogType::info:
		default:
			std::cout << "> ";
			break;
		}

		std::cout << string << std::endl;
	}
}
