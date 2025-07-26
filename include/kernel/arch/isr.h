#pragma once

#include "arch/idt.h"
#include "klibc/stdio.h"

typedef struct registers_t {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rbp, rsi, rdi;
	uint64_t rdx, rcx, rbx, rax;
	uint64_t int_no, err_code;
	uint64_t rip, cs, rflags, rsp;
} registers_t;

__attribute__((noreturn))
void exception_handler(registers_t* r);

enum IRQs {
	IRQ0 = 32,
	IRQ1, IRQ2, IRQ3, IRQ4,
	IRQ5, IRQ6, IRQ7, IRQ8,
	IRQ9, IRQ10, IRQ11, IRQ12,
	IRQ13, IRQ14, IRQ15
};

typedef void (*isr_t)(registers_t* r);
void register_irq(uint8_t vector, isr_t callback);
void irq_handler(registers_t* r);
