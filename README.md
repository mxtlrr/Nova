# Nova
x86_64 monolithic kernel intending to emulate the feel of Windows 9x. 
Nova is not NT-based.


# Build Instructions

## Dependencies
- EDK II OVMF firmware
- Netwide Assembler
- GCC (both host and x86_64-elf crosscompiled)
- `mtools` and `xorriso`.

## Instructions
First, find the EDK2 firmware (usually in `/usr/share/edk2/x64`), and 
copy it to a folder named `ovmf`
```sh
$ mkdir -p ovmf/
$ cp /usr/share/edk2/x64/OVMF_CODE.4m.fd ovmf/
$ cp /usr/share/edk2/x64/OVMF_VARS.4m.fd ovmf/
```

Next, run `make`. This will compile both Nova's bootloader and the 
kernel. It will also create an ISO file named `nova.iso`.
```sh
$ make
```

You can test in QEMU with `make qemu`, which is where you'll need the 
`ovmf` directory. Otherwise, testing on real hardware is as simple as 
flashing a USB / other bootable device with the ISO.