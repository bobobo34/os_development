/* declare multiboot header constants */
.set ALIGN,    1 << 0           # aligns loaded modules on page boundaries
.set MEMINFO,  1 << 1           # provides memory map
.set FLAGS,    ALIGN | MEMINFO  # multiboot 'flag' field
.set MAGIC,    0x1BADB002       # magic number to help bootloader find header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are in multiboot

/*
Declare multiboot header that marks program as kernel. These are all magic values that are documented in multiboot standard. Bootloader will search for this signature in first 8 kiB of kernel file, aligned at 32-bit boundary. Signature is in its own section so the header can be forced to be within the first 8 KiB of the kernel file.
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
multiboot standard does not define value of the stack pointer register (esp) and it is up to the kernel to provide a stack. This allocates room for a small stack by creating a symbol at the bottom of it, then allocatin 16384 bytes for it, and finally creating a symbol at the top. the stack grows downwards on x86. The stack is in its own section so it can be marked nobits, which means the kernel file is smaller because it does not contain an uninitialized stack. the stack on x86 MUST BE 16-byte aligned according to the System V ABI standard and de-facto extensions. The compiler will assume the stack is properly aligned and failure to align the stack will result in UNDEFINED BEHAVIOR, SO DON'T MESS IT UP
*/
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
/*
linker script specifies _start as the entry point to the kernel and the bootloader will jump to this position once the kernel has been loaded. It doesn't make sense to return from this function as the bootloader is gone.
*/
.section .text
.global _start
.type _start, @function
_start:
	/*
	The bootloader has loaded us into 32-bit protected mode on a x86 machine.
	Interrupts are disabled. Paging is disabled.
	The processor state is as defined in the multiboot standard.
	The kernel has full control of the CPU.
	The kernel can only make use of hardware features and any code it provides as part of itself.
	There's no printf function, unless the kernel provides its own <stdio.h> header and a printf implementation.
	There are no security restrictions, no safeguards, no debugging mechanisms, only what the kernel provides itself.
	It has absolute and complete power over the machine.'
	*/

	/* to set up a stack, we set the esp register to point to the top of the stack (it grows downwards on x86 systems). This is necessarily done in assembly as languages such as C cannot function without a stack.
	*/
	mov $stack_top, %esp

	/*
	this is a good place to init crucial processor state before the high-level
	is entered. It's best to minimize the early environment where crucial
	features are offline. note that the processor is not fully initialized
	yet: features such as floating point instructions and instruction set
	extensions are not initialized yet. The GDT should be loaded here. Paging
	should be enabled here. C++ features such as global constructors and
	exceptions will require runtime support to work as well.
	*/
	call _init
	/*Enter the high-level kernel. The ABI requires the stack is 16-byte
	aligned at the time of the call instruction (which afterwards pushes the
	return pointer of size 4 bytes). The stack was originally 16-byte aligned
	above and we've pushed a multiple of 16 bytes to the stack since (pushed
	0 bytes so far), so the alignment has thus been preserved and the call
	is well defined.
	*/
	call kernel_main

	call _fini
	/*
	if the system has nothing more to do, put the computer into an infinite
	loop. to do that:
	1. disable Interrupts with cli (clear interrupt enable in eflags).
	   They are already disabled by bootloader, so this is not needed. Mind
	   that you might later enable interrupts and return from kernel_main
	   (which is sort of nonsensicle to do).
	2. Wait for the next interrupt to arrive with hlt (halt instruction).
	   Since they are disabled, this will lock up the computer.
	3. Jump to the hlt instruction if it ever wakes up due to a non-maskable
	   interrupt occuring or due to system management mode.
	*/

	cli
1:      hlt
	jmp 1b	
/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you impl call tracing.
*/
.size _start, . - _start
