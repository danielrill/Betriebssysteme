.global  main

.text
	
main:
	movl	$len, %edx
	movl  $msg, %ecx
	movl  $1, %ebx
	movl  $4, %eax
	int     $0x80 
	
	movl  $0, %ebx
	movl  $1, %eax
	int     $0x80
	
.data
msg:
	.ascii	"Hello, x86\n"
	len = . - msg
	
#gcc -no-pie welcome.o -o welcome
#PIE is a Position Independent Executable, i.e., one that can be executed at any specific #absolute address, i.e., which for example uses RIP-relative addressing.

# gcc -c example.s -o example.o
# gcc -no-pie example.o -o example
# ./example

