; boot.asm - ToolGrup OS Bootloader (MBR, 512 byte)
[org 0x7c00]
[bits 16]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    mov si, msg_boot
    call print16

    ; Diskten 2. sektörü (kernel) yükle: 0x1000'e oku
    mov ah, 0x02        ; read sectors
    mov al, 16          ; 16 sektör (8KB kernel)
    mov ch, 0           ; cylinder
    mov cl, 2           ; sector 2'den başla
    mov dh, 0           ; head
    mov dl, [boot_drive]
    mov bx, 0x1000      ; hedef ES:BX = 0000:1000
    int 0x13
    jc disk_error

    ; Koruma mod'a geç
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or  eax, 1
    mov cr0, eax
    jmp CODE_SEG:protected_mode

disk_error:
    mov si, msg_err
    call print16
    hlt

print16:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print16
.done:
    ret

[bits 32]
protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Kernel'i 0x1000'den 0x100000'e (1MB) kopyala
    cld
    mov esi, 0x1000
    mov edi, 0x100000
    mov ecx, 8192       ; 8KB / 1 byte kopya (dilim dilim)
    rep movsb

    jmp 0x100000        ; Kernel'e atla!

boot_drive db 0
msg_boot   db "ToolGrup OS bootloader OK...", 13, 10, 0
msg_err    db "DISK HATASI!", 13, 10, 0

; GDT
gdt_start:
    dq 0
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xCF
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510-($-$$) db 0
dw 0xAA55
