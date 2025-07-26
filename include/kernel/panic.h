#pragma once

#include "arch/isr.h"

enum KernelPanicReasons {
	OUT_OF_MEMORY = 0,
	DEBUG
};

extern char* reasons[];
void kernel_panic(uint32_t reason);