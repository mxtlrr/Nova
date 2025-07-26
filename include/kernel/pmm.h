#pragma once

#include <stdint.h>
#include <stddef.h>

#include "bootinfo.h"
#include "klibc/stdio.h"
#include "panic.h"

#define PAGE_SIZE 4096
extern uint32_t frames[256];

#define PMM_INDEX_BIT(a) (a / (8 * 4))
#define PMM_OFFSET_BIT(a) (a % (8 * 4))
#define PMM_TEST_FRAME(frames, frame) (frames[PMM_INDEX_BIT(frame)] & (1 << PMM_OFFSET_BIT(frame)))

void pmm_init(MemoryMap* mmap, uint32_t mmap_size);

uint64_t pmm_allocblock();
void pmm_freeblock(uintptr_t block);

// Allocate n blocks (contigous?)
uint64_t pmm_allocblocks(size_t n);

void pmm_freeblocks(uint64_t block_start, size_t nblocks);


void test_pmm(void);