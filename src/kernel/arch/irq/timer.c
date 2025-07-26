#include "arch/irq/timer.h"

static int ticks = 0;
void pit_callback(registers_t* r){
	ticks++;
}

void init_pit(uint32_t freq){
	uint16_t divisor = 1193182 / freq;
	outb(MODE_CMD, 0x36);
	
	outb(CHANNEL_0, (uint8_t)(divisor & 0xFF));
	outb(CHANNEL_0, (uint8_t)((divisor >> 8) & 0xFF));
	register_irq(IRQ0, pit_callback);
}

uint32_t get_ticks(){
	return ticks;
}