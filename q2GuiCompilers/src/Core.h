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
