#include "arch/isr.h"

void exception_handler(){
	kprintf("wtf exception ???");
	for(;;) asm("cli; hlt");
}