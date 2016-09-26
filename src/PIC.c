#include <ioport.h>

void master_EOI() {
	out8(0x20, (1 << 5));
}

void slave_EOI() {
	out8(0xA0, (1 << 5));
}

void PIC_init() {
	out8(0x20, 1 + (1 << 4));
	out8(0xA0, 1 + (1 << 4));
	out8(0x21, 32);
	out8(0xA1, 40);
	out8(0x21, 1 << 2);
	out8(0xA1, 2);
	out8(0x21, 1);
	out8(0xA1, 1);
	out8(0x21, (1 << 8) - 1);
	out8(0xA1, (1 << 8) - 1);
}
