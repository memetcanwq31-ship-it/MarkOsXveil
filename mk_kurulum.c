/* mk_kurulum.c - markos_kurulum.file uretici (DUZELTILMIS)
 * Format: "TGKF" imzasi | version | payload_uzunlugu(u32) | payload
 */
#include <stdio.h>
#include <string.h>

int main(void) {
    const char* script =
        "# MarkOS Kurulum Betigi v0.2\n"
        "step 1: disk yaz -> os.img @ LBA0\n"
        "step 2: kernel yaz -> @ LBA1\n"
        "step 3: cfg kopyala -> /etc/toolgrup/\n"
        "step 4: 755 arac kayit -> tools.cfg\n"
        "step 5: browser kur -> MarkOS Browser (varsayilan)\n"
        "step 6: onay yaz -> kurulum.bitti\n";

    unsigned long len = strlen(script) + 1;   /* +1: sonlandirici dahil */

    FILE* f = fopen("markos_kurulum.file", "wb");
    if (!f) { perror("dosya acilamadi"); return 1; }

    fwrite("TGKF", 1, 4, f);                  /* imza */
    fputc(0x02, f);                           /* format v2 */

    /* Uzunlugu basa yaz -> kernel kurulum programi payload'u dogru okur */
    unsigned char lenb[4] = {
        (unsigned char)(len       & 0xFF),
        (unsigned char)((len>>8)  & 0xFF),
        (unsigned char)((len>>16) & 0xFF),
        (unsigned char)((len>>24) & 0xFF)
    };
    fwrite(lenb, 1, 4, f);

    fwrite(script, 1, len, f);
    fclose(f);

    printf("markos_kurulum.file olusturuldu (%u byte payload)\n", (unsigned)len);
    return 0;
}
