#pragma once

#include "arch/idt.h"
#include "klibc/stdio.h"

typedef struct registers_t {
	uint64_t rip, cs, rflags, rsp; // Pushed by default on CPU interrupt
} registers_t;

__attribute__((noreturn))
void exception_handler();