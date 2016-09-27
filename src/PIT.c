#include <ioport.h>
#include <PIC.h>
#include <idt_init.h>
#define PIT_COMMAND_PORT 0x43
#define PIT_DATA_PORT 0x40

void PIT_init(void (*f)(int)) {
	out8(PIT_COMMAND_PORT, (2 << 1) | (3 << 4));
	out8(PIT_DATA_PORT, (1 << 8) - 1);
	out8(PIT_DATA_PORT, (1 << 8) - 1);   
	set_listener(32, f);
	master_unhide(0);
}
