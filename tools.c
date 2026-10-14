/* tools.c - ToolGrup OS Araç Yöneticisi
 * 755+ aracı kaydeder, listeler, kullanıcı modunda çalıştırır.
 */
#include "markos_kernel_core.h"

#define MAX_TOOLS 1024
#define CFG_SECT  0x2000  /* tools.cfg'nin okunduğu sektör bloğu (basitlik) */

typedef struct {
    u32  id;
    char name[32];
    char path[64];
    u8   cat;          /* 0=sistem 1=ofis 2=ag 3=arac */
    u8   loaded;
} tool_t;

static tool_t tools[MAX_TOOLS];
static u32 tool_count = 0;

/* Basit cfg satır ayrıştırıcı: "[tool:NNN]", "name =", "path =", "cat =" */
void tool_registry_load(void) {
    char* cfg = (char*)CFG_SECT;
    tool_t* t = 0;
    for (char* p = cfg; *p; p++) {
        if (p[0]=='[' && p[1]=='t') {              /* yeni araç başlığı */
            t = &tools[tool_count];
            t->id = ++tool_count;
        }
        else if (t && p[0]=='n' && p[1]=='a') {    /* name */
            char* v = p + 7; int i=0;
            while (*v && *v != '\n' && i<31) t->name[i++] = *v++;
            t->name[i] = 0;
        }
        else if (t && p[0]=='p' && p[1]=='a') {    /* path */
            char* v = p + 7; int i=0;
            while (*v && *v != '\n' && i<63) t->path[i++] = *v++;
            t->path[i] = 0;
        }
    }
    kputs("[OK] Araç kayıt defteri: ");
    kputs_dec(tool_count);
    kputs(" araç yüklendi\n");
}

void tool_list(void) {
    for (u32 i = 0; i < tool_count; i++) {
        kputs_dec(tools[i].id);
        kputs("  ");
        kputs(tools[i].name);
        kputs("  (");
        kputs(tools[i].path);
        kputs(")\n");
    }
}

/* Aracı bul ve "çalıştır" (ring3 desteği gelince kullanıcı moduna atacak) */
tool_t* tool_run(const char* name) {
    for (u32 i = 0; i < tool_count; i++)
        if (streq(tools[i].name, name)) {
            kputs("Çalıştırılıyor: "); kputs(name); kputs("\n");
            /* TODO: ring3 geçişi + ToolFS'ten binary yükleme */
            return &tools[i];
        }
    kputs("Araç bulunamadi: "); kputs(name); kputs("\n");
    return 0;
}
