/* mk_kurulum.c - markos_kurulum.file uretici
 * Format: "TGKF" imzasi | version | payload
 */
#include <stdio.h>

int main(void) {
    FILE* f = fopen("markos_kurulum.file", "wb");
    /* Imza + surum */
    fwrite("TGKF", 1, 4, f);
    fputc(0x02, f);                    /* kurulum format v2 */

    /* Kurulum betigi (kernel kurulum sirasini okur) */
    const char* script =
        "# MarkOS Kurulum Betigi v0.2\n"
        "step 1: disk yaz -> os.img @ LBA0\n"
        "step 2: kernel yaz -> @ LBA1\n"
        "step 3: cfg kopyala -> /etc/toolgrup/\n"
        "step 4: 755 arac kayit -> tools.cfg\n"
        "step 5: browser kur -> MarkOS Browser (varsayilan)\n"
        "step 6: onay yaz -> kurulum.bitti\n";
    fwrite(script, 1, sizeof(script)+64, f);  /* uzunluk kontrolu derlemede ayarlanir */

    fclose(f);
    printf("markos_kurulum.file olusturuldu\n");
    return 0;
}
