#ifndef __REFLECTION__
#define __REFLECTION__

#include "multiboot_information.h"

#define _D_COMMENT "m#"
#define _D_UCHAR "uc#"
#define _D_CHAR "sc#"
#define _D_SHORT "us#"
#define _D_USHORT "ss#"
#define _D_UINT "ui#"
#define _D_INT "si#"
#define _D_ULONG "ul#"
#define _D_LONG "sl#"
#define _D_STRING "s#"
#define _D_END "e#"

#define STRUCT_STYLE 0
#define CLASS_STYLE 1

extern const char* double_class_prefix;
extern const char* default_class_prefix;
extern const char* default_struct_prefix;

const char* default_prefix(void* p, int n, int style);

struct struct_description {
	void* ptr;
	const char* name;
	const char** fields;
	const char* (*prefix)(void*, int, int);
};
typedef struct struct_description struct_description_t;

struct_description_t header_descriptor(multiboot_header_t* ptr);
struct_description_t info_descriptor(multiboot_info_t* ptr); 
struct_description_t memory_map_entry_descriptor(memory_map_entry_t* ptr);

#endif
