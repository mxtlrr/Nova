%macro isr_err_stub 1
isr_stub_%+%1:
	push byte %1
	jmp stub
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
	push byte 0
	push byte %1
	jmp stub
%endmacro

%macro IRQ 2
irq_stub_%+%1:
	push byte 0
	push byte %2
	jmp i_stub
%endmacro

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

[extern exception_handler]
[extern irq_handler]

%macro x64pushall 0
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp

	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro x64popall 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8

	pop rbp
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

stub:
	x64pushall

	mov rdi, rsp
	call exception_handler

	x64popall
	add rsp, 16
	iretq

i_stub:
	x64pushall

	mov rdi, rsp
	call irq_handler

	x64popall
	add rsp, 8
	
	iretq


[global isr_stub_table]
[global irq_stub_table]

isr_stub_table:
	%assign i 0 
	%rep    32 
		dq isr_stub_%+i
	%assign i i+1 
	%endrep

irq_stub_table:
	%assign i 0 
	%rep    16
		dq irq_stub_%+i
	%assign i i+1 
	%endrep
