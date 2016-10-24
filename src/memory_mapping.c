#include "memory.h"
#include "output.h"

#define GB (1llu << 30)
#define PHYSICAL_ADRESS_FLAG (1<<7)
#define ENTRY_PRESENT_FLAG (1<<0)
#define WRITE_AVALIBLE_FLAG (1<<1)
#define NOT_PRIVILEGED_ONLY_FLAG (1<<2)
#define ADDRESS_MASK (~((1llu << 12) - 1))

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


void reset_PML4_wrapper();

void init_paging() {
	reset_PML4_wrapper();
}

unsigned long long get_PDPT_entry(unsigned long long phys_address) {
	return (phys_address & ADDRESS_MASK) | ENTRY_PRESENT_FLAG | WRITE_AVALIBLE_FLAG | PHYSICAL_ADRESS_FLAG;
}

void reset_PML4(unsigned long long PML4) {
	unsigned long long PML4_desc = *(unsigned long long *)(PML4);
	unsigned long long PDPT_address = PML4_desc & ADDRESS_MASK; // remove flags
	unsigned long long* PDPT_table = (unsigned long long *)PDPT_address;
	for (unsigned long long i = 1; i < 256; i++) {
		PDPT_table[i] = get_PDPT_entry(i * GB);
	}
	
} 
