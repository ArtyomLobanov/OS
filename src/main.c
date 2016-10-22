
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
	//printf("%ms", &header_descriptor(get_initial_multiboot_header()));
	while (1);
}
