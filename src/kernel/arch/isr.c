#include "arch/isr.h"

uint8_t look_at_opcode(uint64_t addr){
	return *(uint8_t*)(addr);
}

void exception_handler(registers_t* r){
	kprintf("Exception %02x (e=%04x) @ RIP=%04x:%x\n", r->int_no, r->err_code, r->cs, r->rip);
	kprintf("RAX=%x RBX=%x RCX=%x RDX=%x\n",   r->rax, r->rbx, r->rcx, r->rdx);
	kprintf("RDI=%x RSI=%x RBP=%x RSP=%x\n",   r->rdi, r->rsi, r->rbp, r->rsp);
	kprintf("R8 =%x R9 =%x R10=%x R11=%x\n",   r->r8, r->r9, r->r10, r->r11);
	kprintf("R12=%x R13=%x R14=%x R15=%x\n\n", r->r12, r->r13, r->r14, r->r15);

	kprintf("At RIP, the faulty instruction was ");
	color = 0xFF0000;
	if(look_at_opcode(r->rip) == 0x48 /* REX.W */){
		uint8_t next = look_at_opcode(r->rip + 1);
		switch(next){
			case 0xCF: // iret
				kprintf("iretq\n");
				break;
			default:
				kprintf("unknown opcode (%#x)\n", next);
				break;
		}
	} else {
		kprintf("??? (%#x)\n", look_at_opcode(r->rip));
	}
	color = 0xFFFFFF;

	kprintf("=== SYSTEM HALTED ===");
	for(;;) asm("cli; hlt");
}

isr_t handlers[256];

void register_irq(uint8_t vector, isr_t callback){
	kprintf("[NOVA][ISR] Registered vector %d\n", vector);
	handlers[vector] = callback;
}

void irq_handler(registers_t* r){
	color = 0x00ff00;
	kprintf("IRQ %d recv!\n", r->int_no);
	color = 0xffffff;
	eoi(r->int_no);
	if(handlers[r->int_no] != 0){
		isr_t h1 = handlers[r->int_no];
		h1(r);
	}
}
