#pragma once

#include <iostream>
#include <ctime>
#include <queue>
#include <functional>
#include <sstream>
#include <unordered_map>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _WIN32_WINNT 0x0500
#include <Windows.h>

#include "log/Log.h"
#include "events/Event.h"

#define LOG_TRACE(...) ::Q2Compilers::Log::trace(__VA_ARGS__)
#define LOG_INFO(...) ::Q2Compilers::Log::info(__VA_ARGS__)
#define LOG_WARNING(...) ::Q2Compilers::Log::warning(__VA_ARGS__)
#define LOG_ERROR(...) ::Q2Compilers::Log::error(__VA_ARGS__)

#define ASSERT(test, ...) if(!(test)) { LOG_ERROR(__VA_ARGS__); __debugbreak(); }
#define ASSERT_N(test, ...) if((test)) { LOG_ERROR(__VA_ARGS__); __debugbreak(); }

namespace Q2Compilers {
	typedef void (*EventCallbackFunc)(std::shared_ptr<Event>);
}
