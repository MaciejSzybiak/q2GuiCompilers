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

#include <iostream>
#include <ctime>
#include <queue>
#include <functional>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iomanip>
#include <filesystem>

#include <Windows.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log/Log.h"
#include "events/Event.h"

#define LOG_TRACE(...) ::Q2Compilers::Log::trace(__VA_ARGS__)
#define LOG_INFO(...) ::Q2Compilers::Log::info(__VA_ARGS__)
#define LOG_WARNING(...) ::Q2Compilers::Log::warning(__VA_ARGS__)
#define LOG_ERROR(...) ::Q2Compilers::Log::error(__VA_ARGS__)

#define ASSERT(test, ...) if(!(test)) { LOG_ERROR(__VA_ARGS__); __debugbreak(); }
#define ASSERT_N(test, ...) if((test)) { LOG_ERROR(__VA_ARGS__); __debugbreak(); }

#define C_PATH_LENGTH MAX_PATH
#define C_ARGS_LENGTH 512
#define PROFILE_EXTENSION_STR ".m1"
#define W_PROFILE_EXTENSION_STR L".m1"

namespace Q2Compilers
{
	typedef void (*EventCallbackFunc)(std::shared_ptr<Event>);
	typedef int (*TextWidthFunc)(const char* text, int len);

	extern wchar_t dataPath[C_PATH_LENGTH];
}
