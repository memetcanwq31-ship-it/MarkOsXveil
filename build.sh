#!/bin/bash
# build.sh - MarkOS.iso ve markos_kurulum.file uretici
# Gereksinim: nasm, gcc-multilib, xorriso (apt install nasm gcc-multilib xorriso)
set -e

echo "[1/6] Bootloader derleniyor..."
nasm -f bin boot.asm -o boot.bin

echo "[2/6] Kernel derleniyor..."
gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -fno-pic \
    -nostdlib -c markos_kernel_core.c -o kernel_core.o
gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -fno-pic \
    -nostdlib -c tools.c -o tools.o
gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -fno-pic \
    -nostdlib -c markos_browser.c -o browser.o
gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -fno-pic \
    -nostdlib -c libc_mini.c -o libc_mini.o

echo "[3/6] ISR stub derleniyor..."
nasm -f elf32 isr_stub.asm -o isr.o

echo "[4/6] Kernel baglaniyor..."
ld -m elf_i386 -T linker.ld -o markos_kernel_core.bin \
   kernel_core.o tools.o browser.o libc_mini.o isr.o --oformat binary

echo "[5/6] Disk imaji olusturuluyor..."
cat boot.bin > os.img
dd if=markos_kernel_core.bin of=os.img bs=512 seek=1 conv=notrunc

echo "[6/6] ISO hazirlaniyor..."
mkdir -p isodir/boot isodir/etc/toolgrup isodir/tools
cp os.img                     isodir/boot/markos.img
cp toolgrup.cfg               isodir/etc/toolgrup/toolgrup.cfg
cp tools.cfg                  isodir/etc/toolgrup/tools.cfg
cp markos_kurulum.file        isodir/
touch isodir/tools/.keep

# El Torito: kendi bootloader'imiz, emulasyonsuz boot
xorriso -as mkisofs \
  -b boot/grub/stage2_eltorito 2>/dev/null || true
xorriso -as mkisofs \
  -no-emul-boot -boot-load-size 4 -boot-info-table \
  -b boot/markos.img -c boot.catalog \
  -o MarkOS.iso isodir

echo "TAMAM: MarkOS.iso ve os.img hazir!"
echo "Test:  qemu-system-i386 -cdrom MarkOS.iso"
