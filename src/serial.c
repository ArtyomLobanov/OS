#include <ioport.h>
 
unsigned short x = 0x3f8;

void serial_init() {
	out8(x+1, 0);
	out8(x+3, (1 << 7));
	out8(x+0, 1);
	out8(x+1, 0);
	out8(x+3, 0);
	
}

void printChar(char c) {
	uint8_t check = in8(x + 5);
	while (((check >> 5) & 1) == 0) {
		check = in8(x + 5);
	}
	out8(x+0, c);
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
