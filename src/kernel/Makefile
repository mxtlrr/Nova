CC 		 := gcc
CFLAGS := -m64 -Wall -Wextra -pedantic -ffreestanding \
					-fno-stack-protector -m64 -mno-red-zone -nostdlib \
					-I../../include -I../../include/kernel \
					-fno-PIC -fPIE

CFILES := $(shell find . -type f -name '*.c')
ASMFILES := $(shell find . -type f -name '*.asm')

OBJECTS    := $(CFILES:.c=.o)
ASMOBJECTS := $(ASMFILES:.asm=.o)

BIN_DIR := ../../bin
TARGET := $(BIN_DIR)/nova.elf
all: $(TARGET)

$(TARGET): $(OBJECTS) $(ASMOBJECTS)
	@objcopy -O elf64-x86-64 -B i386 -I binary font.psf font.o
	ld -o $@ $^ font.o

%.o: %.c
	@echo CC $^
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@echo AS $^
	@nasm -felf64 $< -o $@

clean:
	@rm $(OBJECTS) $(ASMOBJECTS)