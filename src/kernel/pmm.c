#include "pmm.h"

uint32_t frames[256];

void pmm_init(MemoryMap* mmap, uint32_t mmap_size){
	for(uint32_t i = 0; i < mmap_size; i++){
		uint64_t start = mmap->phys_location;
		uint64_t end   = (mmap->phys_location) + (PAGE_SIZE * mmap->size);

		// align to pages
		start &= ~(0xFFF);
		end = (end + PAGE_SIZE) & ~0xFFF;

		// get frames
		uint32_t starting_frame = (start / PAGE_SIZE);
		uint32_t ending_frame   = (end / PAGE_SIZE);

		for(uint32_t i = starting_frame; i < ending_frame; i++)
			frames[(int)(i/32)] |= (1<<(i%32));
	}
}

uint64_t pmm_allocblock(){
	for(uint32_t i = 0; i < 256; i++){ // Look through each index in bitmap
		uint32_t index = frames[i];
		for(uint32_t bit = 0; bit < 32; bit++){
			if(!(index & (1<<bit))){ // we must have found a free bitmap
				frames[(index / (8*4))] |= (1<<(index % (8*4)));
				return index * 4096;
			}
		}
	}
	return 0xDEAD; // could not find!
}

void pmm_freeblock(uintptr_t block){
	uint32_t index = (block == 0x0) ? 0 : (block/4096);
	frames[(index / (8*4))] &= ~(1<<(index % (8*4))); // Clear bit
}


/* ******* */
void test_pmm(void){
	kprintf("Allocating PMM block...");
	uint64_t b = pmm_allocblock();
	kprintf("allocated, block at phys. %08x\n", b);
	kprintf("Freeing PMM block...");
	pmm_freeblock(b);
	kprintf("ok!\n");
}