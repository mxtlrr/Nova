#include <uefi.h>
#include "elf.h"
#include "util.h"

#include "../bootinfo.h"

#define hcf(x) printf(x); \
							 for(;;);

int main(int argc, char** argv){
	ST->ConOut->ClearScreen(ST->ConOut);
	ST->ConOut->SetAttribute(ST->ConOut, EFI_BACKGROUND_BLUE | EFI_WHITE);
	ST->ConOut->ClearScreen(ST->ConOut);
	
	efi_time_t time;
	ST->RuntimeServices->GetTime(&time, NULL);

	printf("Welcome to Nova's Bootloader!\n");
	printf("Booted at %02d:%02d:%02d on %04d-%02d-%02d\n\n",
		time.Hour, time.Minute, time.Second,
		time.Year, time.Month, time.Day);

	///////////////// Get memory map from UEFI
	efi_memory_descriptor_t* mmap = NULL, *mem_desc;
	uintn_t mmap_size=0, map_key=0, desc_size=0;

	efi_status_t status = BS->GetMemoryMap(&mmap_size, NULL, &map_key, &desc_size, NULL);
	if(status != EFI_BUFFER_TOO_SMALL || !mmap_size){
		hcf("Cannot get memory map!\n");
	}
	
	mmap_size += (4 * desc_size); // allocate more just incase the mmap (for whatever reason) 
	mmap = (efi_memory_descriptor_t*)malloc(mmap_size);

	if(!mmap) {
		hcf("unable to allocate memory\n");
	}
	status = BS->GetMemoryMap(&mmap_size, mmap, &map_key, &desc_size, NULL);
	if(EFI_ERROR(status)) {
		hcf("Unable to get memory map\n");
	}

	uint32_t usable = 0;
	for(mem_desc = mmap; (uint8_t*)mem_desc < (uint8_t*)mmap + mmap_size; mem_desc = NextMemoryDescriptor(mem_desc, desc_size)) {
		if(mem_desc->Type == 7) usable++;
	}
	
	MemoryMap* Kmmap = malloc(sizeof(MemoryMap) * usable); // risky!
	uint32_t index=0;
	for(mem_desc = mmap; (uint8_t*)mem_desc < (uint8_t*)mmap + mmap_size; mem_desc = NextMemoryDescriptor(mem_desc, desc_size)) {
		if(mem_desc->Type == 7){
			Kmmap[index++] = (MemoryMap){ .phys_location = mem_desc->PhysicalStart,
						.size = mem_desc->NumberOfPages };
		}
	}

	free(mmap);

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
	printf("Please select a resolution.\n1: 640x480\n2: 1024x768\n3: Biggest your monitor supports (default)\n");
	printf("Choice (03 seconds):\r");
	
	/* Note: On some buggy firmware (?)(may not actually be a bug, I'm not 100% sure if it is -- but on some UEFI systems
	 * ST->ConIn->ReadKeyStroke is actually not implemented?) Instead we need to setup some form of timer to wait a specific
	 * countdown, i.e. 2-4 seconds */
	efi_event_t event;
	status = BS->CreateEvent(EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK,  TimerCallback, NULL, &event);
	if(EFI_ERROR(status)){
		hcf("Cannot make event!");
	}
	
	// event created!
	BS->SetTimer(event, TimerPeriodic, 10000000);
	if(EFI_ERROR(status)){
		hcf("Cannot set timer for 3 seconds.");
	}

	efi_input_key_t* key;
	for(;;){
		if(ST->ConIn->ReadKeyStroke(ST->ConIn, key) != EFI_NOT_READY) break;
		printf("Choice (%02d seconds):\r", 3 - ctr);
		if(ctr == 3) break;
	}
	printf("%c\n", key->UnicodeChar);

	BS->CloseEvent(&event);


	efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	efi_gop_t* gop;
	
	BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);

	efi_gop_mode_info_t* info;
	uintn_t SizeOfInfo, numModes, nativeMode;

  status = gop->QueryMode(gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
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

		.mmap = Kmmap,
		.mmap_entries = usable
	};

	exit_bs();
	asm("cli");
	(*((void(* __attribute__((sysv_abi)))(BootInfo*))(entry)))(&b);
	
	free(Kmmap);

	// The kernel should never return here.
	for(;;) asm("hlt");
}