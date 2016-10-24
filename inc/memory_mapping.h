#ifndef __MEMORY_MAPPING__
#define __MEMORY_MAPPING__


void* to_logic_address(unsigned long long phys_address);

unsigned long long to_phys_address(void* logic_address);

void init_paging();

#endif
