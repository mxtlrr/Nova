#include "klibc/stdio.h"

extern void load_gdt();

void _start(BootInfo* b){
	initialize_nova_fb(b);
	
	load_gdt();
	kprintf("[NOVA] GDT Enabled.\n");

	kprintf("Printf test: %#x, %04d, 0x%04x, %c, %s",
			0xDEADBEEF, 3, 0x123, 0x5A, "cool beans");
	for(;;);
}