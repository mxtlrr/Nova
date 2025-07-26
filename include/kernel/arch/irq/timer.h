#pragma once

#include <stdint.h>

#include "klibc/stdio.h"
#include "arch/isr.h"

enum PIT_IO {
	CHANNEL_0 = 0x40, // rw
	CHANNEL_1 = 0x41, // rw
	CHANNEL_2 = 0x42, // rw
	MODE_CMD  = 0x43, // write-only, read ignored.
};

void pit_callback(registers_t* r);
void init_pit(uint32_t freq);

uint32_t get_ticks();