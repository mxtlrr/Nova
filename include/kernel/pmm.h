#pragma once

#include <stdint.h>
#include <stddef.h>

#include "bootinfo.h"
#include "klibc/stdio.h"

#define PAGE_SIZE 4096
extern uint32_t frames[256];

void pmm_init(MemoryMap* mmap, uint32_t mmap_size);

uint64_t pmm_allocblock();
void pmm_freeblock(uintptr_t block);

void test_pmm(void);