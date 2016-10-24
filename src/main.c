
#include <handlers.h>
#include <desc.h>
#include <ioport.h>
#include <idt_init.h>
#include <PIC.h>
#include <PIT.h>
#include <serial.h>
#include <output.h>
#include <backtrace.h>
#include <memory_map.h>
#include "buddy_alloc.h"
#include <memory.h>
#include <memory_mapping.h>
#include <slab.h>
#include <general_allocator.h>

void memory_alloc_test() {
	printf("\n---------memory tests-------\n");
	status();
	unsigned long long addrs[5];
	unsigned long long sizes[5] = {1, 14, 5235, 10000, 1325345342};

	for (int i = 0; i < 5; i++) {
		addrs[i] = ggmpom(sizes[i]);
		printf("%u) alloc memory test: size = %u, get adress = %u (= %u mod PAGE_SIZE)\n", (i+1), 
			sizes[i], addrs[i], addrs[i] % PAGE_SIZE);
		status();
		printf("\n");
	}	

	for (int i = 0; i < 5; i++) {
		printf("%u) free memory test: size = %u, address = %u\n", (i+1), sizes[i], addrs[i]);
		gttpom(addrs[i]);
		status();
		printf("\n");
	}
}

void slab_alloc_test(int size) {
	printf("\n---------slab tests-------\n");
	slub_allocator_t* alloc = create_slab(size);
	printf("Create slab (size == %u)\n", size);
	void* addrs[10];
	for (int i = 0; i < 10; i++) {
		addrs[i] = slab_alloc(alloc);
		printf("alloc: logic_addr = %llu, phys_addr = %llu, available = %llu\n", 
			(unsigned long long) addrs[i], to_phys_address(addrs[i]), blocks_available(alloc));
	}
	for (int i = 0; i < 10; i++) {
		slab_free(alloc, addrs[i]);
		printf("free: available = %llu\n", blocks_available(alloc));
	}
	printf("\n---------the end-------\n");
}

void general_allocator_test() {
	printf("\n---------general allocator tests-------\n");
	general_allocator_status();
	void* addrs[5];
	unsigned long long sizes[5] = {1, 14, 5, 10000, 1325345};
	for (int i = 0; i < 5; i++) {
		addrs[i] = alloc(sizes[i]);
		printf("alloc: size = %llu, logic_addr = %llu, phys_addr = %llu\n", sizes[i],
			(unsigned long long) addrs[i], to_phys_address(addrs[i]));
	}
	printf("\n");
	general_allocator_status();
	printf("<free all addresses> \n\n");
	for (int i = 0; i < 5; i++) {
		free(addrs[i]);
	}
	general_allocator_status();
	printf("\n---------the end-------\n");
}

void main() {
	cli_command();
	idt_init();
	PIC_init();
	sti_command();
	/*print_multiboot_header();
	print_info_header();
	printf("\n\n\n-------initial_memory_map is---------\n");
	print_default_memory_map();
	printf("\n\n\n-------new_memory_map is---------\n");*/
	print_actual_memory_map();
	slab_alloc_test(4096);
	slab_alloc_test(100);
	general_allocator_test();
	//memory_alloc_test();
	//c++;
	while (1);
}
