
#include <handlers.h>
#include <desc.h>
#include <ioport.h>
#include <idt_init.h>
#include <PIC.h>
#include <PIT.h>
#include <serial.h>
#include <output.h>
#include <backtrace.h>




void generate();

void foo(int n) {
	n++;
	print("Timer works!\n");
}

void bar() {
	backtrace();
}
void f() {
	bar();
}

void main(void) {
	cli_command();
	idt_init();
	PIC_init();
	sti_command();

	// serial test
	print("Hello, World!\n"); 
	printChar('\n');

	// idt & interrupt test
	generate();
	printChar('\n');
	
	// backtrace test
	f();
	printChar('\n');

	// fprintf test
	char res[35];
	snprintf(&res[0], 35, "%liab%ig%i  %s%hhi\n", -2342243532245LL, 5, 6, "snprintf", 'I');
	print(&res[0]);
	printf("%uqwerty%lli\nprintf", 13243, 6754443423564126LL);
	printChar('\n');

	// timer test
	PIT_init(foo);
	while (1);
}
