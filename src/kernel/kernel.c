#include <stdint.h>
#include "bootinfo.h"

void _start(BootInfo* b){
	*(uint32_t*)(b->Framebuffer.addr) = 0xFF0000;
	for(;;);
}