#include "klibc/stdio.h"

void _start(BootInfo* b){
	initialize_nova_fb(b);
	kputc("[NOVA] Hello world!\n");
	for(;;);
}