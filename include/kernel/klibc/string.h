#pragma once

#include <stdint.h>
#include <stddef.h>

int   memcmp(uint8_t* str1, uint8_t* str2, size_t n);
void* memmove(void* dest, void* src, size_t n);
void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);