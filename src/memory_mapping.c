#include "memory.h"


void* to_logic_address(unsigned long long phys_address) {
	return (void*) (phys_address + VIRTUAL_BASE);
}

unsigned long long to_phys_address(void* logic_address) {
	unsigned long long adr = (unsigned long long) logic_address;
	if (adr >= VIRTUAL_BASE) {
		adr -= VIRTUAL_BASE;
	}
	return adr;
}
