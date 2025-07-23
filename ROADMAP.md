# Nova Roadmap

# Phase 0: Baby Steps

## 0.0: Bootloader
- [ ] Setup framebuffer (allow user to select between a couple different
modes, get address to buffer, etc)
- [ ] Use EFI boot services to get:
	- [ ] Memory Map provided by firmware
- [ ] Load kernel (ELF file)

## 0.1: Kernel Baby Steps
- [ ] Setup both GDT and IDT
	- [ ] Install a proper ISR handler
	- [ ] Enable IRQs
- [ ] Memory management (PMM for now, VMM later.)
- [ ] Initial ramdisk + VFS

# Phase 1: Getting Further

## 1.0: User space
- [ ] Jump into userspace
	- [ ] Write syscall API
- [ ] Load programs (ELF files, maybe PE?)
- [ ] Write a shell

## Phase 2-$n$: TBD
TBD