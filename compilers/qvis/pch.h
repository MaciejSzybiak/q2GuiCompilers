#pragma once

#define printf(x, ...) printf_hack(x, __VA_ARGS__)
int printf_hack(const char* format, ...);
