#include "arch/idt.h"
#include "arch/isr.h"

#include "klibc/stdio.h"

extern void load_gdt();

void _start(BootInfo* b){
	initialize_nova_fb(b);
	
	load_gdt();	kprintf("[NOVA] GDT Enabled.\n");
	load_idt();	kprintf("[NOVA] IDT Enabled.\n");

	for(;;) asm("hlt");
}
