#include "arch/isr.h"

uint8_t look_at_opcode(uint64_t addr){
	return *(uint8_t*)(addr);
}

void exception_handler(registers_t* r){
	kprintf("Exception %02x (e=%04x) @ RIP=%04x:%08x\n", r->int_no, r->err_code, r->cs, r->rip - 1);
	kprintf("RAX=%08x RBX=%08x RCX=%08x RDX=%08x\n", r->rax, r->rbx, r->rcx, r->rdx);
	kprintf("RDI=%08x RSI=%08x RBP=%08x RSP=%08x\n\n", r->rdi, r->rsi, r->rbp, r->rsp);

	kprintf("=== SYSTEM HALTED ===");
	for(;;) asm("cli; hlt");
}
