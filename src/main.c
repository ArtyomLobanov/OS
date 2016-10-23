
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

void main() {
	cli_command();
	idt_init();
	PIC_init();
	sti_command();
	print_multiboot_header();
	print_info_header();
	printf("\n\n\n-------initial_memory_map is---------\n");
	print_default_memory_map();
	printf("\n\n\n-------new_memory_map is---------\n");
	print_actual_memory_map();
	
	memory_alloc_test();
	
	while (1);
}
