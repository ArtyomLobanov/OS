#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <stdarg.h>
#include <stddef.h>
#include "reflection.h"

int vprintf(const char* fmt, va_list args);

int vsnprintf(char* s, size_t n, const char* fmt, va_list args);

int snprintf(char* s, size_t n, const char* fmt, ...);

int printf(const char* fmt, ...);

#endif
