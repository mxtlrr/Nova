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

		// Mark all indexes as free, we're not using them.
		for(uint32_t i = starting_frame; i < ending_frame; i++)
			frames[(int)(i/32)] |= (0<<(i%32));
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

	kernel_panic(OUT_OF_MEMORY);
	return 0;
}

void pmm_freeblock(uintptr_t block){
	uint32_t index = (block == 0x0) ? 0 : (block/4096);
	frames[(index / (8*4))] &= ~(1<<(index % (8*4))); // Clear bit
}

uint64_t pmm_allocblocks(size_t n) {
	if (n == 0) return 0x0;

	for (uint32_t i = 0; i < 256; i++) {
		uint32_t index = frames[i];
		if (index != 0xFFFFFFFF) { // found one
			for (uint32_t bit = 0; bit < 32; bit++) {
				if (!(index & (1 << bit))) {
					int start = i * 32 + bit;
					uint32_t free_bits = 0;
					for (uint32_t check = 0; check < n; check++) { // check if enough
						if (!PMM_TEST_FRAME(frames, start + check)) free_bits++;
						if (free_bits == n){
							uint32_t frame = i * 4 * 8 + bit;

							// Allocate these blocks in the bitmap
							for(uint32_t i = 0; i < n; i++)
								frames[((frame+i) / (8*4))] |= (1<<((frame+i) % 8*4));
							return (uintptr_t)(frame*4096);
						}
					}
				}
			}
		}
	}

	kernel_panic(OUT_OF_MEMORY);
	return 0;
}

void pmm_freeblocks(uint64_t block_start, size_t nblocks){
	uint32_t index = (block_start == 0) ? 0 : (block_start / 4096);
	for(uint32_t i = 0; i < nblocks; i++){
		frames[((index+i) / (8*4))] &= ~(1<<((index+i) % (8*4)));
	}
}

/* **** Make sure the PMM is actually working *** */
void test_pmm(void){
	kprintf("Testing PMM singular allocate...\n");
	uint64_t b = pmm_allocblock();
	kprintf("   Allocated. Physical Address at %08x\n", b);
	kprintf("   Freeing PMM block..."); pmm_freeblock(b);
	kprintf("ok!\n");

	kprintf("Testing PMM multiple allocate (3 blocks)...\n");
	uint64_t b2 = pmm_allocblocks(3);
	kprintf("   Allocated. Physical Address at %08x\n", b2);
	kprintf("   Freeing..."); pmm_freeblocks(b2, 3);
	kprintf("done!\n");

	if(b == b2) kprintf("Sanity check passed!\n");
	else {
		kprintf("Sanity check failed. Something is wrong with PMM. Bye bye.\n");
		kernel_panic(DEBUG);
	}

	color = 0x00ff00;
	kprintf("==== PMM TEST COMPLETE ====\n\n");
	color = 0xffffff;
}