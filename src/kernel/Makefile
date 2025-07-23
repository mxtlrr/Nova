CC 		 := gcc
CFLAGS := -m64 -Wall -Wextra -pedantic -ffreestanding \
					-fno-stack-protector -m64 -mno-red-zone -nostdlib
LDFLAGS := -lgcc

CFILES := $(shell find . -type f -name '*.c')
OBJECTS := $(CFILES:.c=.o)

BIN_DIR := ../../bin
TARGET := $(BIN_DIR)/nova.elf
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@echo CC $^
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm $(OBJECTS)