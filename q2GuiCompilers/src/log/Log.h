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

#define LOG_FN(type) static void type(const char * format, ...) \
					 { \
						 char buf[1024]; \
						 va_list args; \
						 va_start(args, format); \
						 vsnprintf(buf, sizeof(buf), format, args); \
						 va_end(args); \
						 LogMessage(LogType::##type, buf); \
					 } \

namespace Q2Compilers
{
	enum class LogType
	{
		trace, info, warning, error, assert
	};

	class Log
	{
	public:
		static void Init();
		static std::string& GetLogString()
		{
			static std::string buf;
			buf = buffer.str();
			return buf;
		}
		LOG_FN(trace);
		LOG_FN(info);
		LOG_FN(warning);
		LOG_FN(error);
		LOG_FN(assert);
	private:
		static std::stringstream buffer;
		static void LogMessage(LogType type, const char* string);
		static HANDLE GetOutputHandle()
		{
			static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			return handle;
		}
	};
}
