#pragma once

typedef struct {
	uintptr_t phys_location;
	uintptr_t size;
} MemoryMap;

typedef struct {
	struct Framebuffer {
		uint64_t addr;
		uint32_t pitch;
		uint16_t width, height;
	} Framebuffer;

	MemoryMap* mmap;
	uint32_t mmap_entries; // Usable memory map entries
} BootInfo;