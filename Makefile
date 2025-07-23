OUTPUT := nova

all: build_nova build_img

build_nova:
	@make -C src/boot
	@make -C src/kernel

build_img:
	dd if=/dev/zero of=$(OUTPUT).img bs=512 count=93750
	mformat -i $(OUTPUT).img ::
	mmd -i $(OUTPUT).img ::/EFI
	mmd -i $(OUTPUT).img ::/EFI/BOOT
	mcopy -i $(OUTPUT).img bin/nova.elf ::
	mcopy -i $(OUTPUT).img bin/BOOTX64.EFI ::/EFI/BOOT
	mkdir -p iso/
	cp $(OUTPUT).img iso
	xorriso -as mkisofs -R -f -e $(OUTPUT).img -no-emul-boot -o $(OUTPUT).iso iso || exit
	@rm -rf iso/

clean:
	@rm *.img *.iso
	@make -C src/boot clean
	@make -C src/kernel clean

qemu:
	@qemu-system-x86_64 -cpu qemu64 -drive if=pflash,format=raw,unit=0,file=ovmf/OVMF_CODE.4m.fd,readonly=on \
										-drive if=pflash,format=raw,unit=1,file=ovmf/OVMF_VARS.4m.fd \
										-drive file=$(OUTPUT).img,if=ide -net none