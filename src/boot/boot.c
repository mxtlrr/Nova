#include <uefi.h>
#include "elf.h"
#include "../bootinfo.h"

#define hcf(x) printf(x); \
							 for(;;);

int main(int argc, char** argv){
	ST->ConOut->ClearScreen(ST->ConOut);


	///////////////// Load kernel

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

	///////////////// Setup GOP framebuffer.

	/////// Let's ask the user which one they want to select.
	printf("Please select a resolution.\n1: 640x480\n2: 1024x768\n3: Biggest your monitor supports (default)\nChoice: ");
	
	efi_input_key_t* key;
	while(ST->ConIn->ReadKeyStroke(ST->ConIn, key) == EFI_NOT_READY);
	printf("I got: %c\n", key->UnicodeChar);


	efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	efi_gop_t* gop;
	
	BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);

	efi_gop_mode_info_t* info;
	uintn_t SizeOfInfo, numModes, nativeMode;

  efi_status_t status = gop->QueryMode(gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
  // this is needed to get the current video mode
  if (status == EFI_NOT_STARTED) gop->SetMode(gop, 0);
	nativeMode = gop->Mode->Mode;
	numModes = gop->Mode->MaxMode;

	uint32_t width, mode = 0;
	for(uint32_t i = 0; i < numModes; i++){
		gop->QueryMode(gop, i, &SizeOfInfo, &info);
		if(key->UnicodeChar == '1'){ // 640x480
			if(info->HorizontalResolution == 640 && info->VerticalResolution == 480){
				mode = i;
				break;
			}
		} else if(key->UnicodeChar == '2') { // 1024x768
			if(info->HorizontalResolution == 1024 && info->VerticalResolution == 768){
				mode = i;
				break;
			}
		} else {
			if(info->HorizontalResolution > width) mode = i;
		}
	}
	gop->SetMode(gop, i);
	
	// Call kernel
	BootInfo b = {
		.Framebuffer = {
			.addr = gop->Mode->FrameBufferBase,
			.pitch = 4 * gop->Mode->Information->PixelsPerScanLine,
			.width = gop->Mode->Information->HorizontalResolution,
			.height = gop->Mode->Information->VerticalResolution
		},
	};

	exit_bs();
	asm("cli");
	(*((void(* __attribute__((sysv_abi)))(BootInfo*))(entry)))(&b);

	// The kernel should never return here.
	for(;;) asm("hlt");
}