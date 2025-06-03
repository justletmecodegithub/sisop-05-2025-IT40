prepare:
	dd if=/dev/zero of=bin/floppy.img bs=512 count=2880

bootloader:
	nasm -f bin bootloader.asm -o bin/bootloader.bin
dd if=bin/bootloader.bin of=bin/floppy.img conv=notrunc

stdlib:
	bcc -ansi -c -o bin/std_lib.o std_lib.c

shell:
	bcc -ansi -c -o bin/shell.o shell.c

kernel:
	bcc -ansi -c -o bin/kernel.o kernel.c
	nasm -f elf kernel.asm -o bin/kernel_asm.o

link:
	dd if=bin/bootloader.bin of=bin/floppy.img conv=notrunc
	dd if=bin/kernel.o of=bin/floppy.img seek=1 conv=notrunc
	dd if=bin/kernel_asm.o of=bin/floppy.img seek=2 conv=notrunc
	dd if=bin/std_lib.o of=bin/floppy.img seek=3 conv=notrunc
	dd if=bin/shell.o of=bin/floppy.img seek=4 conv=notrunc

build: prepare bootloader stdlib shell kernel link

clean:
	rm -f bin/*.o bin/*.bin bin/floppy.img
