
#include <handlers.h>
#include <desc.h>
#include <ioport.h>
#include <idt_init.h>
#include <PIC.h>
#include <serial.h>



void generate();
void sti_command();
void cli_command();

void PIT_init() {
	out8(0x43, (2 << 1) | (3 << 4));
	out8(0x40, (1 << 8) - 1);
	out8(0x40, (1 << 8) - 1);    
	out8(0x21, ((1 << 7) - 1) << 1);
}


void main(void) {
	serial_init();
	idt_init();
	print("Hello, World!\n");
	cli_command();
	generate();
	PIC_init();
	sti_command();
	PIT_init();
	print("I'm here!\n");
	print("I'm here! again\n");
	while (1);
}
