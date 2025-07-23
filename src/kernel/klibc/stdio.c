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
	
	for (int y = 0; y < height-16; y++) {
		uint8_t *dst = fb2 + (y * bytes_per_line);
		uint8_t *src = fb2 + ((y+16) * bytes_per_line);
		memcpy(dst, src, bytes_per_line);
	}

	for (int y = height-16; y < height; y++) {
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