#ifndef __GENERAL_ALLOCATOR__
#define __GENERAL_ALLOCATOR__

void* alloc(unsigned long long size);
void free(void* ptr);
void general_allocator_status();
#endif
