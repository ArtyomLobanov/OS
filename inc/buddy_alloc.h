#ifndef __BUDDY_ALLOC__
#define __BUDDY_ALLOC__

void init_multy_buddy_alloc();

unsigned long long ggmpom(unsigned long long page_count);

void gttpom(unsigned long long phys_address);

unsigned long long free_pages_number();

void status();

#endif
