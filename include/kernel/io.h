#pragma once

#include <stdint.h>
#include <stddef.h>

#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

/* 8259 PIC stuff */
void eoi(uint8_t irq);
void remap(uint8_t irq0, uint8_t irq8);
