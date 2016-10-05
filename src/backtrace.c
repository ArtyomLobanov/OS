#include <output.h>

extern unsigned long long bootstrap_stack_top;


int check_rbp(unsigned long long* rbp) {
	while((unsigned long long) rbp >= bootstrap_stack_top) {
		if ((unsigned long long) rbp >= *rbp && *rbp >= bootstrap_stack_top)
			return 0;
		rbp = (unsigned long long*) (*rbp);
	}
	return 1;
}
void print_backtrace(unsigned long long* rbp) {
	if (!check_rbp(rbp)) {
		printf("Error: invalid rbp\n");
		return;
	}
	printf("Backtrace:\n the currently executing function\n");
	int flag = 0;
	while ((unsigned long long) rbp >= bootstrap_stack_top) {
		printf("  %scalled from %llx\n", (flag? "which ": ""), rbp[1]);
		rbp =(unsigned long long*) (*rbp);
		flag = 1;
	}
	printf("\n");
}

	

