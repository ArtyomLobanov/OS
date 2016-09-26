
#include <handlers.h>
#include <desc.h>
#include <ioport.h>
#include <idt_init.h>
#include <PIC.h>
#include <PIT.h>
#include <serial.h>



void generate();
void sti_command();
void cli_command();

void main(void) {
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
