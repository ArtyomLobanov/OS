
#include <handlers.h>
#include <desc.h>
#include <ioport.h>
#include <idt_init.h>

unsigned short x = 0x3f8;
unsigned char idt[16 * 256];

struct desc_table_ptr ptr_idt = {0, 0};


void generate();
void sti_command();
void cli_command();


void idt_init() {
	ld_all();
	ptr_idt.size = 256 * 16 - 1;
	ptr_idt.addr = (unsigned long long)(&idt);
	write_idtr(&ptr_idt);
}

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

void master_EOI() {
	out8(0x20, (1 << 5));
}

void slave_EOI() {
	out8(0xA0, (1 << 5));
}
 
void work(int n) {
	print("Work with interupt №");
	printInt(n);
	print("\n");
	if (n >= 32 && n < 48) {
		master_EOI();
	}
	if (n >= 40) {
		slave_EOI();
	}
}


void main(void) {
	serial_init();
	idt_init();
	write_idtr(&ptr_idt);
	print("Hello, World!\n");
	cli_command();
	generate();
	PIC_init();
	sti_command();
	print("I'm here!\n");
	print("I'm here! again\n");
	while (1);
}
