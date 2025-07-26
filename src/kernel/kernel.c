#include "klibc/stdio.h"

#include "arch/idt.h"
#include "arch/irq/timer.h"

extern void load_gdt();

void _start(BootInfo* b){
	initialize_nova_fb(b);
	
	load_gdt();	kprintf("[NOVA] GDT Enabled.\n");
	load_idt();	kprintf("[NOVA] IDT Enabled.\n");

	asm("int $0x3");
	init_pit(50); // Initialize PIT Timer IRQ
	for(;;) asm("hlt");
}
