CC      = i686-elf-gcc        # veya gcc -m32 -ffreestanding
AS      = nasm
LD      = i686-elf-ld

boot.bin: boot.asm
	$(AS) -f bin boot.asm -o boot.bin

kernel.o: markos_kernel_core.c
	$(CC) -m32 -ffreestanding -fno-builtin -fno-stack-protector \
	      -nostdlib -c markos_kernel_core.c -o kernel.o

isr.o: isr_stub.asm
	$(AS) -f elf32 isr_stub.asm -o isr.o

kernel.bin: kernel.o isr.o
	$(LD) -m elf_i386 -T linker.ld -o kernel.bin kernel.o isr.o \
	      --oformat binary

os.img: boot.bin kernel.bin
	cat boot.bin > os.img
	dd if=kernel.bin of=os.img bs=512 seek=1 conv=notrunc

clean:
	rm -f *.bin *.o os.img

run: os.img
	qemu-system-i386 -drive format=raw,file=os.img
