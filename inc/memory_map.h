#ifndef __MEMORY_MAP__
#define __MEMORY_MAP__

#include "multiboot_information.h"

void print_multiboot_header();
void print_info_header();
void print_default_memory_map();
void print_actual_memory_map();
void init();

#define MAX_MEMORY_MAP_CAPACITY 100
extern memory_map_entry_t memory_map[MAX_MEMORY_MAP_CAPACITY];
extern int memory_map_size;

#endif
