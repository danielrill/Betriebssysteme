#void swtch(struct context **old, struct context *new);
#
# Save current register context in old
# and than load register constext from new
.globl swtch

swtch:
	# Save old registers
	movl 4(%esp), %eax		#put old ptr into eax
	popl 0(%eax)			#save the old IP
	movl %esp, 4(%eax)		#and stack
	movl %ebx, 8(%eax)		#and other registers
	movl %ecx, 12(%eax)
	movl %edx, 16(%eax)
	movl %esi, 20(%eax)
	movl %edi, 24(%eax)
	movl %ebp, 28(%eax)

	# Load new registers
	movl 4(%esp), %eax		#put new ptr into eax
	movl 28(%eax), %ebp		#restore other registers
	movl 24(%eax), %edi
	movl 20(%eax), %esi
	movl 16(%eax), %edx
	movl 12(%eax), %exc
	movl 8(%eax), %ebx
	movl 4(%eax), %esp		#stack is switched here

	pushl 0(%esp)			#return addres put in place
	ret						#return into new context

# eax = extended AX, AX now refers to the lower half of EAX, while AH and AL
# continue to refer to the two AX bytes
# ax =  was a 16-bit accumulator(ein Register in das Ergebnise gespeichert werden)
# while AH and AL could be tought of as
# 8-bit registers on their own or as a way to access the high-order or low-order bytes of AX
# esp = extended stack pointer

# 		in x86-64 the the extended registers get an R prefix, thus RAX,64-bit
# 	|<----------------------------------- RAX, 64 bit ---------------------------------->|
#	   |						  				                 	|<------------- EAX, 32 bit ------------>|
#    |											                 |   		   |<-------- AX, 16 bit ------->|
#	   |										                    |    		   |<- AH, 8bit ->|<- AL, 8bit ->|
#	   |									                	RAX |	   EAX |		           AH |		       AL |

#  From an 8-bit A 		of 8008
#	  to  16-bit AX		of 8086
#	  to  32-bit EAX 	of 80386
#	  to  64-bit RAX
