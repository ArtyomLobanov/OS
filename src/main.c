
#include <handlers.h>
#include <desc.h>
#include <ioport.h>
#include <idt_init.h>
#include <PIC.h>
#include <PIT.h>
#include <serial.h>



void generate();

void foo(int n) {
	n++;
	print("Timer works!\n");
}


void main(void) {
	cli_command();
	idt_init();
	print("Hello, World!\n");
	generate();
	PIC_init();
	sti_command();
	PIT_init(foo);
	print("\nThe End!\n");
	while (1);
}
