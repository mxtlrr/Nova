#include "panic.h"

char* reasons[] = {
	"Out of Memory (PMM)",
	"Debug"
};

void kernel_panic(uint32_t reason){
	color = 0xFF0000;
	kprintf("\nKERNEL PANIC! NOVA HAS BEEN HALTED!\n");
	color = 0xFFFFFF;
	kprintf("Reason: %s\n", reasons[reason]);
	kprintf("Please reboot your machine. Nova cannot continue in this state.\n");
	
	
	for(;;){
		asm("cli");
		asm("hlt");
	}
}