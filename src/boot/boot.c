#include <uefi.h>
#include "elf.h"

int main(int argc, char** argv){
	ST->ConOut->ClearScreen(ST->ConOut);

	FILE* fp; uint64_t size; uint8_t* buf;
	if(fp = fopen("nova.elf", "r"), !fp) for(;;);
	
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if(buf = malloc(size+1), !buf) for(;;);
	fread(buf, size, 1, fp);
	fclose(fp);

	Elf64_Ehdr* elf = (Elf64_Ehdr*)buf;
	Elf64_Phdr* phdr;
	// Honestly I could really just check if we're a valid ELF, but
	// eh.
	
	int i; uintptr_t entry;
	for(phdr = (Elf64_Phdr*)(buf + elf->e_phoff), i = 0;
		i < elf->e_phnum; i++, phdr = (Elf64_Phdr*)((uint8_t*)phdr + elf->e_phentsize)){
		if(phdr->p_type == 1 /* pt_load*/){
			// load segments
			memcpy((void*)phdr->p_vaddr, buf + phdr->p_offset, phdr->p_filesz);
			memset((void*)(phdr->p_vaddr + phdr->p_filesz), 0, phdr->p_memsz - phdr->p_filesz);
		}
	}
	entry = elf->e_entry;
	free(buf);
	
	// Call kernel
	(*((void(* __attribute__((sysv_abi)))(void))(entry)))();

	// The kernel should never return here.
	for(;;) asm("hlt");
}