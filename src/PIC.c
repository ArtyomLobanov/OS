#include <ioport.h>
#define MASTER_COMMAND_PORT 0x20 
#define SLAVE_COMMAND_PORT 0xA0
#define MASTER_DATA_PORT 0x21 
#define SLAVE_DATA_PORT 0xA1

unsigned char master_mask;
unsigned char slave_mask;

void master_EOI() {
	out8(MASTER_COMMAND_PORT, (1 << 5));
}

void slave_EOI() {
	out8(MASTER_COMMAND_PORT, (1 << 5));
}


void master_hide(int port) {
	master_mask |= (1 << port);
	out8(MASTER_DATA_PORT, master_mask);
}

void slave_hide(int port) {
	slave_mask |= (1 << port);
	out8(SLAVE_DATA_PORT, slave_mask);
}

void master_unhide(int port) {
	master_mask &= ~(1 << port);
	out8(MASTER_DATA_PORT, master_mask);
}

void slave_unhide(int port) {
	slave_mask &= ~(1 << port);
	out8(SLAVE_DATA_PORT, slave_mask);
}

void master_hide_all() {
	master_mask = (1 << 8) - 1;
	out8(MASTER_DATA_PORT, master_mask);
}

void slave_hide_all() {
	slave_mask = (1 << 8) - 1;
	out8(SLAVE_DATA_PORT, slave_mask);
}

void PIC_init() {
	out8(MASTER_COMMAND_PORT, 1 + (1 << 4));
	out8(SLAVE_COMMAND_PORT, 1 + (1 << 4));
	out8(MASTER_DATA_PORT, 32);
	out8(SLAVE_DATA_PORT, 40);
	out8(MASTER_DATA_PORT, 1 << 2);
	out8(SLAVE_DATA_PORT, 2);
	out8(MASTER_DATA_PORT, 1);
	out8(SLAVE_DATA_PORT, 1);
	master_hide_all();
	slave_hide_all();
}

