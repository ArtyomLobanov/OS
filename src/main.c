
#include <handlers.h>
#include <desc.h>
#include <ioport.h>
#include <idt_init.h>
#include <PIC.h>
#include <PIT.h>
#include <serial.h>
#include <output.h>
#include <backtrace.h>



void start64();
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
	printf("help-info:\n f - %llx\n main - %llx\n start64 - %llx\n", f, main, start64);
	f();
	printChar('\n');

	// fprintf test
	char res[35];
	snprintf(&res[0], 35, "%liab%ig%i  %s%hhi%c\n", -2342243532245LL, 5, 6, "snprintf", 'I', 'I');
	print(&res[0]);
	printf("%uqwerty%lli\nprintf", 13243, 6754443423564126LL);
	long long l = -9223372036854775807LL;
	l--;
	printf("\n%lli", l);
	printChar('\n');

	// timer test
	PIT_init(foo);
	while (1);
}
