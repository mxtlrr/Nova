#include "klibc/string.h"

int memcmp(uint8_t* str1, uint8_t* str2, size_t n){
  size_t counter = 0;
  for(size_t i = 0; i < n; i++){
    if(str1[i] == str2[i]) counter++;
  }
  
  return (counter == n) ? 0 : ((counter == 0) ? 0 : counter);
}

void* memmove(void* dest, void* src, size_t n){
  uint8_t* p1 = (uint8_t*)dest;
  uint8_t* p2 = (uint8_t*)src;

  if(src>dest)
    for(size_t i = 0; i < n; i++) p1[i] = p2[i];
  else if(src<dest)
    for(size_t i = n; i > 0; i--) p2[i-1] = p1[i-1];
  return dest; 
}

void* memset(void* s, int c, size_t n){
  uint8_t* p = (uint8_t*)s;
  for(size_t i = 0; i < n; i++) p[i] = (uint8_t)c;
  return s;
}

void *memcpy(void* dest, const void* src, size_t n) {
  uint8_t* pdest = (uint8_t*)dest;
  const uint8_t* psrc = (const uint8_t*)src;

  for (size_t i = 0; i < n; i++) pdest[i] = psrc[i];
  return dest;
}

uint32_t strlen(char* f){
  uint32_t i=0;
  for(; f[i] != '\0'; i++);
  return i;
}

char* itoa(unsigned int num, int base){ 
	static char repr[]= "0123456789abcdef";
	static char buffer[50]; 
	char *ptr; 

	ptr = &buffer[49]; 
	*ptr = '\0'; 

	do {
		*--ptr = repr[num%base]; 
		num /= base; 
	}while(num != 0); 

	return(ptr); 
}