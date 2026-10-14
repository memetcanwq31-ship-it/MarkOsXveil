#ifndef KERNEL_H
#define KERNEL_H
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void kclear(void); void kputc(char c); void kputs(const char*);
void kputs_hex(u32); void kputs_dec(u32);
void* kmalloc(u32);
void tool_registry_load(void); void tool_list(void);
tool_t* tool_run(const char*);  /* tools.h'dan gelir */
void markos_browser_start(void); void browser_cmd(const char*);
int strncmp(const char*, const char*, u32);
int snprintf_out(char*, u32, const char*, ...);
#endif
