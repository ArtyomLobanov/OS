.code64
.global backtrace
.extern print_backtrace

backtrace:
	movq %rbp, %rdi
	cld
	call print_backtrace
	ret

.global reset_PML4_wrapper
.extern reset_PML4

reset_PML4_wrapper:
	movq %cr3, %rdi
	cld
	call reset_PML4
	ret 
