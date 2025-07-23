#pragma once

typedef struct {
	struct Framebuffer {
		uint64_t addr;
		uint32_t pitch;
		uint16_t width, height;
	} Framebuffer;
} __attribute__((packed)) BootInfo;