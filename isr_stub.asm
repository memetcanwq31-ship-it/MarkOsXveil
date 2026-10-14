[bits 32]
global isr_stub
extern kmain   ; sadece referans, handler panik mesajı basar

isr_stub:
    cli
    hlt
    jmp isr_stub
