#pragma once

#include "arch/idt.h"
#include "klibc/stdio.h"

typedef struct registers_t {
	uint64_t rbp, rsi, rdi;
	uint64_t rdx, rcx, rbx, rax;
	uint64_t int_no, err_code;
	uint64_t rip, cs, rflags, rsp;
} registers_t;

__attribute__((noreturn))
void exception_handler(registers_t* r);
