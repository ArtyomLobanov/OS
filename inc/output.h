#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <stdarg.h>
#include <stddef.h>

#define _DESCRIPTION_NOTHING 1
#define _DESCRIPTION_UCHAR 2
#define _DESCRIPTION_CHAR 3
#define _DESCRIPTION_SHORT 4
#define _DESCRIPTION_USHORT 5
#define _DESCRIPTION_UINT 6
#define _DESCRIPTION_INT 7
#define _DESCRIPTION_ULONG 8
#define _DESCRIPTION_LONG 9
#define _DESCRIPTION_STRING 10
struct struct_description {
	void* ptr;
	const char* name;
	int fields_counter;
	const char** fields;
	int* types;
	int (*prefix)(void*, int);
};
typedef struct struct_description struct_description_t;

int vprintf(const char* fmt, va_list args);

int vsnprintf(char* s, size_t n, const char* fmt, va_list args);

int snprintf(char* s, size_t n, const char* fmt, ...);

int printf(const char* fmt, ...);

#endif
