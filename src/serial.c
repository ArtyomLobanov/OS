#include <ioport.h>
#define SERIAL_BASE_PORT 0x3f8

static int ready = 0;

void serial_init() {
	ready = 1;
	out8(SERIAL_BASE_PORT+1, 0);
	out8(SERIAL_BASE_PORT+3, (1 << 7));
	out8(SERIAL_BASE_PORT+0, 1);
	out8(SERIAL_BASE_PORT+1, 0);
	out8(SERIAL_BASE_PORT+3, 0);
	
}

void printChar(char c) {
	if (!ready)
		serial_init();
	uint8_t check = in8(SERIAL_BASE_PORT + 5);
	while (((check >> 5) & 1) == 0) {
		check = in8(SERIAL_BASE_PORT + 5);
	}
	out8(SERIAL_BASE_PORT+0, c);
}

void print(char* m) {
	while (*m != 0) {
		printChar(*m);
		m++;
	}
}

void printInt(int c) {
	char buf[19];
	for (int i = 0; i < 18; i++) {
		buf[i] = c%10;
		c /= 10;
	} 
	for (int i = 17; i >= 0; i--) {
		printChar('0' + buf[i]);
	}
}
