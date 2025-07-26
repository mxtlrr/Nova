# Nova Roadmap

# Phase 0: Baby Steps

## 0.0: Bootloader
- [X] Setup framebuffer (allow user to select between a couple different modes, get address to buffer, etc)
- [X] Use EFI boot services to get:
	- [X] Memory Map provided by firmware
- [X] Load kernel (ELF file)
- [ ] Load an initial ramdisk from ESP into memory somewhere

## 0.1: Kernel Baby Steps
- [X] Setup both GDT and IDT
	- [X] Install a proper ISR handler
	- [X] Enable IRQs
- [ ] Memory management
	- [X] PMM (bitmap allocator)
	- [ ] VMM
	- [ ] Paging
- [ ] Initial ramdisk + VFS

# Phase 1: Getting Further

## 1.0: User space
- [ ] Jump into userspace
	- [ ] Write syscall API
- [ ] Load programs (ELF files, maybe PE?)
- [ ] Write a shell

## Phase 2-n: TBD
TBD