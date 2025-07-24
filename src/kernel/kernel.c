#include "klibc/stdio.h"

extern void load_gdt();

void _start(BootInfo* b){
	initialize_nova_fb(b);
	
	load_gdt();
	kputs("[NOVA] GDT Enabled!\n");
	for(;;);
}