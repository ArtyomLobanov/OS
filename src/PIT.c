#include <ioport.h>

void PIT_init() {
	out8(0x43, (2 << 1) | (3 << 4));
	out8(0x40, (1 << 8) - 1);
	out8(0x40, (1 << 8) - 1);    
	out8(0x21, ((1 << 7) - 1) << 1);
}
