/* data/uretec.c */
#include <stdio.h>
#include <stdlib.h>

const char* SANATCILAR[] = {
    "Radiohead", "Daft Punk", "Portishead", "Massive Attack",
    "Boards of Canada", "The National", "Bon Iver", "Burial",
    "Four Tet", "Aphex Twin", "Bicep", "Floating Points",
    "Moderat", "Nils Frahm", "Bonobo", "Tycho",
    "Caribou", "Nicolas Jaar", "James Blake", "Burial"
};

const char* ALBUMLER[] = {
    "OK Computer", "Discovery", "Dummy",
    "Geogaddi", "Alligator", "For Emma",
    "Mezzanine", "Untrue",
    "There Is Love in You", "Selected Ambient Works"
};

void veri_uret(const char* dosya, int n) {
    srand(42); /* sabit seed: her çalıştırmada aynı veriyi üretir */
    FILE* f = fopen(dosya, "w");
    if (!f) {
        printf("HATA: %s olusturulamadi!\n", dosya);
        return;
    }
    
    fprintf(f, "id,baslik,sanatci,album,sure_sn\n");
    
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,Track_%05d,%s,%s,%d\n",
                i, i,
                SANATCILAR[rand() % 20],
                ALBUMLER[rand() % 10],
                120 + rand() % 240);
    }
    
    fclose(f);
    printf("%d satir uretildi: %s\n", n, dosya);
}

int main(int argc, char* argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 100000;
    
    /* CSV dosyası data klasörünün içine oluşturulacak */
    veri_uret("sarkilar.csv", n); 
    return 0;
}
