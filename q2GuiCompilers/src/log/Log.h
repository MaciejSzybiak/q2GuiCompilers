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
