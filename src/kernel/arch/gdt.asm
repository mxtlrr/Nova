[bits 64]
[global load_gdt]
[section .text]

load_gdt:
	cli
	
	lgdt [gdtr]

	push CODESEL64
	lea rax, [rel .flush]
	push rax
	retfq	
	
.flush:
	mov rax, DATASEL64
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

gdt:
	.gdt_null:
		dq 0
	.gdt_code:
		dw 0xFFFF		  ;; Limit
		dw 0x0000	    ;; Base
		db 0x00	  		;; Base
		db 0x9A  			;; Access byte
		db 0b10101111 ;; High 4 -> Flags / low 4 -> limt
		db 0
	
	.gdt_data:
		dw 0xFFFF
		dw 0x0000
		db 0x00
		db 0x92
		db 0b11001111 ;; Flags: 0xC
		db 0
	
	;; TODO: user mode code/data
	.end:

gdtr:
	dw gdt.end - gdt - 1
	dq gdt

CODESEL64 equ gdt.gdt_code - gdt
DATASEL64 equ gdt.gdt_data - gdt