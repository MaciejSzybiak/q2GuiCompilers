#include "pch.h"
namespace qvisn
{
    int printf_hack(const char* format, ...)
    {
        int result;
        char buf[1024];
        va_list args;
        va_start(args, format);
        result = vsnprintf(buf, sizeof(buf), format, args);
        va_end(args);

        std::cout << buf;

        return result;
    }
}
