#ifndef __IDT_H__
#define __IDT_H__

void idt_init();
void set_listener(int iterrupt_number, void (*f)(int));

#endif
