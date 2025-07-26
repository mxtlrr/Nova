#include "klibc/stdio.h"

uint64_t fb = 0;
uint32_t pitch = 0, width = 1024, height = 768;
uint32_t color = 0xFFFFFF;
int x = 0, y = 0;

void setcolor(uint32_t c) {
	color = c;
}

void initialize_nova_fb(BootInfo* b){
	fb    = b->Framebuffer.addr;
	pitch = b->Framebuffer.pitch;
	width = b->Framebuffer.width; height = b->Framebuffer.height;
}

void putpixel(int x, int y, uint32_t color){
	*((uint32_t*)(fb + pitch * y + 4 * x)) = color;
}

void putglyph(int glyph, int x, int y){
	int pX = x, pY = y;
	unsigned char* glyphh = (unsigned char*)&_binary_font_psf_start +
			4 + (16 * glyph);
	
			for(uint8_t i = 0; i < 16; i++){
		uint8_t byte = glyphh[i];
		for(uint8_t j = 8; j != 0; j--){ // bit
			if(byte & (1<<j)){
				putpixel(pX, pY, color);
			}
			pX++;
		}
		pY++;
		pX = x;
	}
}

void scroll() {
	uint32_t bytes_per_pixel = 4; // 24-bit = 3 bytes per pixel
	uint32_t bytes_per_line = width * bytes_per_pixel;
	uint8_t *fb2 = (uint8_t*)fb;
	
	for (uint32_t y = 0; y < height-16; y++) {
		uint8_t *dst = fb2 + (y * bytes_per_line);
		uint8_t *src = fb2 + ((y+16) * bytes_per_line);
		memcpy(dst, src, bytes_per_line);
	}

	for (uint32_t y = height-16; y < height; y++) {
		uint8_t *dst = fb2 + (y * bytes_per_line);
		memset(dst, 0, bytes_per_line);
	}

	x = 0;
	y = height-16;
}


void kputc(char c){
	if((uint32_t)y >= height) scroll();

	switch(c){
		case '\n':
			x = 0;
			y += 16;
			break;
		case '\r':
			x = 0;
			break;
		default: {
			putglyph(c,x,y);
			if((uint32_t)(x+8) >= width){
				x = 0;
				y += 16;
			} else (x += 8);
			break;
		}
	}
}

void kputs(char* fmt){
	for(int i = 0; fmt[i] != '\0'; i++) kputc(fmt[i]);
}

void kprintf(char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);

	char* ptr;
	for(ptr = fmt; *ptr != '\0';){
		if(*ptr == '%'){
			++ptr;
			if(*ptr == '0'){
				char n = (*++ptr) - 0x30;
				char f = (*++ptr);
				switch(f){
					case 'd': {
						uint32_t number = va_arg(ap, uint32_t);
						uint32_t digit_count = ((uint32_t)n) - strlen(itoa(number, 10));
						if(number != 0) for(uint32_t i = 0; i < digit_count; i++) kputc('0');
						else for(uint32_t i = 1; i < (uint32_t)n; i++) kputc('0');
						kputs(itoa(number, 10));
						break;
					}

					case 'x': {
						uint32_t number = va_arg(ap, uint32_t);
						uint32_t digit_count = ((uint32_t)n) - strlen(itoa(number, 16));
						if(number != 0) for(uint32_t i = 0; i < digit_count; i++) kputc('0');
						else for(uint32_t i = 1; i < (uint32_t)n; i++) kputc('0');
						kputs(itoa(number, 16));
						break;
					}
				}
			} else if(*ptr == '#'){
				char n = (*++ptr);
				switch(n){
					case 'x':
						kputs("0x");	
						kputs(itoa(va_arg(ap, uint32_t), 16));
						break;
				}
			} else {
				switch(*ptr){
					case 'x':
						kputs(itoa(va_arg(ap, uint32_t), 16));
						break;

					case 'd':
						kputs(itoa(va_arg(ap, uint32_t), 10));
						break;
					
					case 'c':
						kputc(va_arg(ap, int));
						break;
					
					case 's':
						kputs(va_arg(ap, char*));
						break;
				}
			}
		} else {
			kputc(*ptr);
		}
		*ptr++;
	}
}
