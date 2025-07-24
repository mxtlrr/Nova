#pragma once

#include <stddef.h>
#include <stdint.h>

#define INTR_GATE 0x8E
#define TRAP_GATE 0x8F

typedef struct {
	uint16_t offset1;
	uint16_t sel;
	uint8_t ist;
	uint8_t type_attr;
	uint16_t offset2;
	uint32_t offset3;
	uint32_t zero;
} __attribute__((packed)) idt_desc64;

typedef struct {
	uint16_t lim;
	uint64_t base;
} __attribute__((packed)) idtr_t;

extern void* isr_stub_table[];
extern uint8_t vectors[256];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void load_idt(void);
