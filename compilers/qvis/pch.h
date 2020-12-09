#pragma once

#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#define printf(x, ...) printf_hack(x, __VA_ARGS__)
	int printf_hack(const char* format, ...);

