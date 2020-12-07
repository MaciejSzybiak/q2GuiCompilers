#pragma once

#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#define printf(x, ...) printf_hack(x, __VA_ARGS__)
int printf_hack(const char* format, ...);

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#ifndef __BYTEBOOL__
#define __BYTEBOOL__
//typedef enum {} qboolean;
typedef unsigned char byte;
#endif

#include <algorithm>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>

class qboolean final {
public:
    qboolean() = default;
    qboolean(const qboolean&) = default;
    qboolean(bool b) { m_value = (int)b; }
    qboolean(int i) { m_value = i; }
    operator int() { return m_value; }
private:
    int m_value;
};
static_assert(sizeof(qboolean) == 4, "");
typedef unsigned char byte;

struct vec3_t {
    union {
        struct { float x, y, z; };
        float data[3];
    };
};

typedef float vec_t;
