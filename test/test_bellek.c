#include <stdio.h>
#include "../src/ministream.h"
#include "../src/bellek_izci.h"

void test_toplu_olustur_sil() {
    printf("\n[BELLEK TEST] toplu olustur/sil\n");
    izci_sifirla();
    for (int i = 0; i < 10000; i++) {
        Sarki* s = sarki_olustur(i, "Test", "Sanatci", "Album", 180);
        izlened_free(s, sizeof(Sarki));
    }
    bellek_raporu_yazdir();
}

void test_liste_bellek() {
    printf("\n[BELLEK TEST] liste bellek yonetimi\n");
    izci_sifirla();
    Sarki* sarkilar[100];
    for (int i = 0; i < 100; i++) {
        sarkilar[i] = sarki_olustur(i, "Track", "Artist", "Album", 180);
    }

    for (int i = 0; i < 100; i++) {
        CalmaListesi* l = liste_olustur(i, "Liste");
        for (int j = 0; j < 50; j++) {
            liste_sarki_ekle(l, sarkilar[j % 100]);
        }
        liste_temizle(l);
    }

    for (int i = 0; i < 100; i++) {
        sarki_sil(sarkilar[i]);
    }
    bellek_raporu_yazdir();
}

int main() {
    printf("===== MiniStream Bellek Testleri =====\n");
    test_toplu_olustur_sil();
    test_liste_bellek();
    printf("\n======================================\n");
    return 0;
}
