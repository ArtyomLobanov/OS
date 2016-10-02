.code64
.global backtrace
.extern print_backtrace
.extern print_header

backtrace:
movq %rbp, %rdi
cld
call print_backtrace
ret
