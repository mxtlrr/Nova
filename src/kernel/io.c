#include "io.h"

void outb(uint16_t port, uint8_t val){
	asm volatile("outb %b0, %w1" :: "a"(val), "Nd"(port) : "memory");
}

uint8_t inb(uint16_t port){
  uint8_t ret;
  asm volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}


void eoi(uint8_t irq){
	if(irq >= 8) outb(0xA0, 0x20);
	outb(0x20, 0x20);
}

void remap(uint8_t irq0, uint8_t irq8){
	outb(PIC1, ICW1);
	outb(PIC2, ICW1);

	// remap pics
	outb(PIC1+1, irq0);
	outb(PIC2+1, irq8);

	outb(PIC1+1,4); // ICW3
	outb(PIC2+1,2);

	outb(PIC1+1, ICW4);
	outb(PIC2+1, ICW4);
}
