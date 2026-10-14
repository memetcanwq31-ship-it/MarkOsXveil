/* markos_kernel_core.c - ToolGrup OS Kernel
 * Tamamen bağımsız (freestanding), libc'siz, Linux'sız çekirdek.
 */
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

/* ---- Ekran sürücüsü (VGA text 0xB8000) ---- */
static volatile u16* const VGA = (volatile u16*)0xB8000;
static int cur_row = 0, cur_col = 0;

static u16 vga_entry(char c, u8 fg, u8 bg) {
    return (u16)c | ((u16)(bg << 4 | fg) << 8);
}

void kclear(void) {
    for (int i = 0; i < 80 * 25; i++) VGA[i] = vga_entry(' ', 7, 0);
    cur_row = cur_col = 0;
}

void kputc(char c) {
    if (c == '\n') { cur_row++; cur_col = 0; }
    else if (c == '\r') { cur_col = 0; }
    else {
        VGA[cur_row * 80 + cur_col] = vga_entry(c, 10, 0); /* açık yeşil */
        if (++cur_col >= 80) { cur_col = 0; cur_row++; }
    }
    if (cur_row >= 25) { cur_row = 24; /* basit kaydırma yok, üstüne yaz */ }
}

void kputs(const char* s) { while (*s) kputc(*s++); }

void kputs_hex(u32 n) {
    const char* h = "0123456789ABCDEF";
    kputs("0x");
    for (int i = 28; i >= 0; i -= 4) kputc(h[(n >> i) & 0xF]);
}

/* ---- CPU port I/O ---- */
static inline void outb(u16 port, u8 val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* ---- PIC: kesme denetleyicisi remap + maske ---- */
#define PIC1_CMD 0x20
#define PIC1_DAT 0x21
void pic_init(void) {
    outb(PIC1_CMD, 0x11);      /* ICW1 */
    outb(PIC1_DAT, 0x20);      /* ICW2: offset 0x20 */
    outb(PIC1_DAT, 0x04);      /* ICW3 */
    outb(PIC1_DAT, 0x01);      /* ICW4 */
    outb(PIC1_DAT, 0xFF);      /* tüm IRQ'ları maskele (şimdilik) */
}

/* ---- GDT + IDT (çekirdek kendi tablolarını kurar) ---- */
struct gdt_ptr { u16 limit; u32 base; } __attribute__((packed));
static u64 gdt[3];

void gdt_init(void) {
    gdt[0] = 0;
    gdt[1] = 0x00CF9A000000FFFFULL; /* kod: base 0, limit 4GB */
    gdt[2] = 0x00CF92000000FFFFULL; /* veri */
    struct gdt_ptr gp = { sizeof(gdt) - 1, (u32)gdt };
    __asm__ volatile("lgdt %0" : : "m"(gp));
    __asm__ volatile(
        "mov ax, 0x10\n mov ds, ax\n mov es, ax\n"
        "mov fs, ax\n mov gs, ax\n mov ss, ax\n"
        "jmp 0x08:1f\n1:\n" : : : "ax");
}

struct idt_entry { u16 lo, sel; u8 zero, flags; u16 hi; } __attribute__((packed));
struct idt_ptr  { u16 limit; u32 base; } __attribute__((packed));
static struct idt_entry idt[256];
static struct idt_ptr idtp;

extern void idt_default_isr(void); /* geciken : tek asm stub, tamamına aşağıda */

void idt_set(int n, u32 handler, u8 flags) {
    idt[n].lo = handler & 0xFFFF;
    idt[n].hi = (handler >> 16) & 0xFFFF;
    idt[n].sel = 0x08;
    idt[n].zero = 0;
    idt[n].flags = flags;
}

void idt_init(void) {
    for (int i = 0; i < 256; i++) idt_set(i, (u32)isr_stub, 0x8E);
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (u32)idt;
    __asm__ volatile("lidt %0" : : "m"(idtp));
}

/* ---- Basit heap (bump allocator) ---- */
static u32 heap_ptr = 0x200000; /* 2MB'dan başla */
void* kmalloc(u32 size) {
    void* p = (void*)heap_ptr;
    heap_ptr += (size + 15) & ~15; /* 16-byte align */
    return p;
}

/* ---- Basit komut yorumlayıcı (ToolGrup Shell) ---- */
void shell(void) {
    kputs("\nToolGrup OS v0.1 - Ozgur ve bagimsiz dagitim\n");
    kputs("Komutlar: help | info | clear | mem\n");
    kputs("toolgrup> ");
}

/* ---- Kernel giriş noktası ---- */
void kmain(void) {
    kclear();
    kputs("==========================================\n");
    kputs("   TOOLGRUP OS - markos_kernel_core.c\n");
    kputs("   Linux'tan tamamen bagimsiz ozgur OS\n");
    kputs("==========================================\n\n");

    gdt_init();
    idt_init();
    pic_init();
    __asm__ volatile("sti");

    kputs("[OK] GDT yuklendi\n");
    kputs("[OK] IDT yuklendi\n");
    kputs("[OK] PIC remap tamam\n");
    kputs("[OK] VGA text mod aktif\n");
    kputs("[OK] Heap baslangic: "); kputs_hex(0x200000); kputs("\n");

    shell();
    for (;;) __asm__ volatile("hlt");
}
