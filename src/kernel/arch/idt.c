#include "arch/idt.h"


__attribute__((aligned(0x10)))
static idt_desc64 idt[256];

static idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
	idt_desc64* descriptor = &idt[vector];

	descriptor->offset1        = (uint64_t)isr & 0xFFFF;
	descriptor->sel			       = 0x08;
	descriptor->ist            = 0;
	descriptor->type_attr      = flags;
	descriptor->offset2        = ((uint64_t)isr >> 16) & 0xFFFF;
	descriptor->offset3        = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
	descriptor->zero           = 0;
}

void load_idt(void){
	idtr.base = (uint64_t)&idt[0];
	idtr.lim  = (uint16_t)(sizeof(idt_desc64) * 256) - 1;

	for(uint8_t vector = 0; vector < 32; vector++){
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
	}

	// load idt and set interrupt flag
	asm("lidt %0" :: "m"(idtr));
	asm("sti");
}
