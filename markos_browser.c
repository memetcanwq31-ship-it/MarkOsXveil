/* markos_browser.c - MarkOS Browser
 * Google'suz, bağımsız tarayıcı. Motor: markhtml, Ağ: marknet.
 */
#include "markos_kernel_core.h"

#define URL_MAX 128

/* ---- markhtml: kendi HTML motorumuz ---- */
/* Desteklenen etiketler: <html> <head> <title> <body> <h1..h3> <p> <b> <a> <br> */
typedef enum { R_TITLE, R_BODY, R_LINK, R_TEXT } mode_t2;
static mode_t2 mode;

void markhtml_render(const char* html) {
    kputs("\n--- MarkOS Browser (markhtml motoru) ---\n");
    const char* p = html;
    char buf[256]; int bi = 0;
    while (*p) {
        if (*p == '<') {
            if (bi) { buf[bi]=0; kputs(buf); bi=0; }
            if (!strncmp(p, "<title>", 7)) mode = R_TITLE;
            else if (!strncmp(p, "<h1>", 4)) { kputs("\n== "); }
            else if (!strncmp(p, "<p>", 3))  kputs("\n");
            else if (!strncmp(p, "<br>", 4)) kputs("\n");
            else if (!strncmp(p, "<a href=\"", 9)) {
                p += 9;
                kputs("[baglanti: ");
                while (*p && *p != '"') kputc(*p++);
                kputs("] ");
                while (*p && *p != '>') p++;
            }
            while (*p && *p != '>') p++; /* etiketi atla */
            p++;
            continue;
        }
        buf[bi++] = *p++;
        if (bi >= 255) { buf[bi]=0; kputs(buf); bi=0; }
    }
    if (bi) { buf[bi]=0; kputs(buf); }
    kputs("\n--- son ---\n");
}

/* ---- marknet: kendi ağ yığını iskeleti ---- */
/* Katmanlar: NIC sürücüsü -> Ethernet -> IP -> TCP -> HTTP */
#define HTTP_PORT 80

typedef struct { u8 ip[4]; u16 port; } mhost_t;

/* Basit GET isteği üret (paket gönderme ring3+NIC sürücüsü ile tamamlanır) */
void marknet_http_get(const char* host, const char* path, char* out, u32 max) {
    int n = snprintf_out(out, max,
        "GET %s HTTP/1.0\r\nHost: %s\r\n"
        "User-Agent: MarkOS Browser/0.1\r\n\r\n",
        path, host);
    /* TODO: TCP el sıkışması + NIC TX ring (marknet v0.2) */
}

/* ---- Tarayıcı ana döngüsü ---- */
void markos_browser_start(void) {
    kputs("======================================\n");
    kputs("  MARKOS BROWSER v0.1\n");
    kputs("  Motor: markhtml | Ag: marknet\n");
    kputs("  Google YOK - Sadece MarkOS\n");
    kputs("======================================\n");
    kputs("adres: markos://start\n");
    /* markos://start = yerel sayfa, ağ olmadan bile açılır */
    markhtml_render(
        "<html><head><title>MarkOS Baslangic</title></head>"
        "<body><h1>MarkOS Browser</h1>"
        "<p>ToolGrup OS'un ozgur tarayicisina hos geldiniz.</p>"
        "<a href=\"markos://hakkinda\">Hakkinda</a>"
        "</body></html>");
}

/* Shell komutu olarak bağlan: "browser <adres>" */
void browser_cmd(const char* url) {
    if (!strncmp(url, "markos://", 9)) {
        markos_browser_start();  /* yerel sayfalar */
        return;
    }
    kputs("marknet: harici site - TCP yigini v0.2'de aktif olacak\n");
}
