
#include <handlers.h>
#include <desc.h>
#include <ioport.h>
#include <idt_init.h>
#include <PIC.h>
#include <PIT.h>
#include <serial.h>
#include <output.h>




void generate();

void foo(int n) {
	n++;
	print("Timer works!\n");
}

void main(void) {
	cli_command();
	idt_init();
	print("Hello, World!\n");
	generate();
	PIC_init();
	sti_command();
	char res[35];
	snprintf(&res[0], 35, "%liab%ig%i  %s%hhi\n", -2342243532245LL, 5, 6, "snprintf", 'I');
	print(&res[0]);
	printf("%uqwerty%lli\nprintf", 13243, 6754443423564126LL);
	PIT_init(foo);
	print("\nThe End!\n");
	while (1);
}
