#ifndef __PIC_H__
#define __PIC_H__

void master_EOI();

void slave_EOI();

void PIC_init();

void master_hide(int port);

void slave_hide(int port);

void master_unhide(int port);

void slave_unhide(int port);

void master_hide_all();

void slave_hide_all();


#endif
