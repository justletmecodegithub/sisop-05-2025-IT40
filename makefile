AS = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

$(shell mkdir -p $(BIN_DIR))

ASFLAGS_BOOT = -f bin
ASFLAGS_KERNEL_ASM = -f elf
CFLAGS = -m32 -ffreestanding -nostdlib -Wall -Wextra -c -I$(INC_DIR)
LDFLAGS_KERNEL = -m elf_i386 -Ttext 0x1000 -nostdlib
OBJCOPYFLAGS = -O binary

BOOTLOADER_BIN = $(BIN_DIR)/bootloader.bin
KERNEL_ELF = $(BIN_DIR)/kernel.elf
KERNEL_BIN = $(BIN_DIR)/kernel.bin
FLOPPY_IMG = $(BIN_DIR)/floppy.img

STD_LIB_O = $(BIN_DIR)/std_lib.o
SHELL_O = $(BIN_DIR)/shell.o
KERNEL_C_O = $(BIN_DIR)/kernel.o
KERNEL_ASM_O = $(BIN_DIR)/kernel_asm.o

KERNEL_OBJECTS = $(KERNEL_C_O) $(KERNEL_ASM_O) $(STD_LIB_O) $(SHELL_O)

.PHONY: all clean prepare bootloader_build stdlib_build shell_build kernel_c_build kernel_asm_build kernel_build link_elf convert_to_bin create_image build

all: build

build: $(FLOPPY_IMG)
	@echo "===> EorzeOS build completed successfully!"
	@echo "===> Final image: $(FLOPPY_IMG)"

$(FLOPPY_IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@echo "===> Creating final floppy image: $(FLOPPY_IMG)..."
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $(FLOPPY_IMG)

prepare:
	@echo "===> Preparing empty floppy disk image (1.44MB)..."
	dd if=/dev/zero of=$(FLOPPY_IMG) bs=512 count=2880 status=none

$(BOOTLOADER_BIN): $(SRC_DIR)/bootloader.asm
	@echo "===> Assembling bootloader: $< -> $@"
	$(AS) $(ASFLAGS_BOOT) $< -o $@

$(STD_LIB_O): $(SRC_DIR)/std_lib.c $(INC_DIR)/std_lib.h $(INC_DIR)/std_type.h
	@echo "===> Compiling stdlib: $< -> $@"
	$(CC) $(CFLAGS) $< -o $@

$(SHELL_O): $(SRC_DIR)/shell.c $(INC_DIR)/shell.h $(INC_DIR)/kernel.h $(INC_DIR)/std_lib.h $(INC_DIR)/std_type.h
	@echo "===> Compiling shell: $< -> $@"
	$(CC) $(CFLAGS) $< -o $@

$(KERNEL_C_O): $(SRC_DIR)/kernel.c $(INC_DIR)/kernel.h $(INC_DIR)/shell.h $(INC_DIR)/std_type.h
	@echo "===> Compiling kernel C code: $< -> $@"
	$(CC) $(CFLAGS) $< -o $@

$(KERNEL_ASM_O): $(SRC_DIR)/kernel.asm
	@echo "===> Assembling kernel ASM code: $< -> $@"
	$(AS) $(ASFLAGS_KERNEL_ASM) $< -o $@

$(KERNEL_ELF): $(KERNEL_OBJECTS)
	@echo "===> Linking kernel object files into ELF: $@"
	$(LD) $(LDFLAGS_KERNEL) -o $@ $(KERNEL_OBJECTS)

$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "===> Converting ELF to raw binary: $< -> $@"
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

bootloader_build: $(BOOTLOADER_BIN)
stdlib_build: $(STD_LIB_O)
shell_build: $(SHELL_O)
kernel_c_build: $(KERNEL_C_O)
kernel_asm_build: $(KERNEL_ASM_O)
kernel_build: kernel_c_build kernel_asm_build # Atau langsung $(KERNEL_C_O) $(KERNEL_ASM_O)
link_elf: $(KERNEL_ELF)
convert_to_bin: $(KERNEL_BIN)
create_image: $(FLOPPY_IMG)

clean:
	@echo "===> Cleaning build directory: $(BIN_DIR)..."
	rm -rf $(BIN_DIR)
	@echo "===> Clean complete."
