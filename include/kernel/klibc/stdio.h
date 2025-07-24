#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "klibc/string.h"
#include "bootinfo.h"

extern char _binary_font_psf_start[];
extern uint32_t color;

void initialize_nova_fb(BootInfo* b);
void putpixel(int x, int y, uint32_t color);
void setcolor(uint32_t c);

void kputc(char c);
void kputs(char* fmt);
void kprintf(char* fmt, ...);