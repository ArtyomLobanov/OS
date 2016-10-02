#include <output.h>

extern unsigned long long bootstrap_stack_top;

void print_backtrace(unsigned long long* rbp) {
	printf("Backtrace:\n the currently executing function\n");
	int flag = 0;
	while ((unsigned long long) rbp >= bootstrap_stack_top) {
		printf("  %scalled from %llx\n", (flag? "which ": ""), rbp[1]);
		rbp =(unsigned long long*) (*rbp);
		flag = 1;
	}
	printf("\n");
}
	

