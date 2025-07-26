#include "arch/isr.h"

uint8_t look_at_opcode(uint64_t addr){
	return *(uint8_t*)(addr);
}

void exception_handler(registers_t* r){
	kprintf("Exception %02x (e=%04x) @ RIP=%04x:%08x\n", r->int_no, r->err_code, r->cs, r->rip);
	kprintf("RAX=%08x RBX=%08x RCX=%08x RDX=%08x\n",   r->rax, r->rbx, r->rcx, r->rdx);
	kprintf("RDI=%08x RSI=%08x RBP=%08x RSP=%08x\n",   r->rdi, r->rsi, r->rbp, r->rsp);
	kprintf("R8 =%08x R9 =%08x R10=%08x R11=%08x\n",   r->r8, r->r9, r->r10, r->r11);
	kprintf("R12=%08x R13=%08x R14=%08x R15=%08x\n\n", r->r12, r->r13, r->r14, r->r15);

	kprintf("At RIP, the faulty instruction was ");
	color = 0xFF0000;
	uint64_t faulty_inst = (r->int_no == 0x03) ? (r->rip - 1) : r->rip;
	if(look_at_opcode(faulty_inst) == 0x48 /* REX.W */){
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
		// TODO: look for other prefixes
		if(look_at_opcode(faulty_inst) == 0xCC){
			kprintf("int3\n");
		} else kprintf("??? (%#x)\n", look_at_opcode(faulty_inst));
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
	if(handlers[r->int_no] != 0){
		isr_t h1 = handlers[r->int_no];
		h1(r);
	}
	eoi(r->int_no);
}
