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
