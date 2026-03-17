#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/ministream.h"
#include "../src/bellek_izci.h"

double ms_olc(struct timespec t1, struct timespec t2) {
    return (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_nsec - t1.tv_nsec) / 1e6;
}

void kopya_modeli_test(int n_sarki, int n_liste, int sarki_per_liste) {
    Sarki** havuz = (Sarki**)izlenen_malloc(n_sarki * sizeof(Sarki*));
    for(int i=0; i<n_sarki; i++) {
        havuz[i] = sarki_olustur(i, "Havuz Sarki", "Sanatci", "Album", 180);
    }

    CalmaListesi** listeler = (CalmaListesi**)izlenen_malloc(n_liste * sizeof(CalmaListesi*));
    for (int i = 0; i < n_liste; i++) {
        listeler[i] = liste_olustur(i, "Kopya Liste");
        for (int j = 0; j < sarki_per_liste; j++) {
            if (listeler[i]->sarki_sayisi >= listeler[i]->kapasite) {
                listeler[i]->kapasite *= 2;
                listeler[i]->sarkilar = (Sarki**)realloc(listeler[i]->sarkilar, listeler[i]->kapasite * sizeof(Sarki*));
            }
            // KOPYA MODELİ: Şarkının aynısını RAM'de yeniden yaratıyoruz
            Sarki* kopya = (Sarki*)izlenen_malloc(sizeof(Sarki));
            memcpy(kopya, havuz[rand() % n_sarki], sizeof(Sarki));
            kopya->ref_sayisi = 0;
            kopya->sonraki = NULL;
            listeler[i]->sarkilar[listeler[i]->sarki_sayisi++] = kopya;
        }
    }

    // Temizlik: Kopyaları, listeleri ve havuzu sil
    for(int i=0; i<n_liste; i++) {
        for(int j=0; j<listeler[i]->sarki_sayisi; j++) {
            izlened_free(listeler[i]->sarkilar[j], sizeof(Sarki));
        }
        liste_temizle(listeler[i]);
    }
    for(int i=0; i<n_sarki; i++) izlened_free(havuz[i], sizeof(Sarki));
    izlened_free(havuz, n_sarki * sizeof(Sarki*));
    izlened_free(listeler, n_liste * sizeof(CalmaListesi*));
}

void pointer_modeli_test(int n_sarki, int n_liste, int sarki_per_liste) {
    Sarki** havuz = (Sarki**)izlenen_malloc(n_sarki * sizeof(Sarki*));
    for(int i=0; i<n_sarki; i++) {
        havuz[i] = sarki_olustur(i, "Havuz Sarki", "Sanatci", "Album", 180);
    }

    CalmaListesi** listeler = (CalmaListesi**)izlenen_malloc(n_liste * sizeof(CalmaListesi*));
    for (int i = 0; i < n_liste; i++) {
        listeler[i] = liste_olustur(i, "Pointer Liste");
        for (int j = 0; j < sarki_per_liste; j++) {
            // POINTER MODELİ: Sadece adresi kopyalıyoruz (ekstra yer ayırmak yok!)
            liste_sarki_ekle(listeler[i], havuz[rand() % n_sarki]);
        }
    }

    // Temizlik: Listeleri ve havuzu sil
    for(int i=0; i<n_liste; i++) {
        liste_temizle(listeler[i]);
    }
    for(int i=0; i<n_sarki; i++) {
        if(havuz[i]->ref_sayisi == 0) izlened_free(havuz[i], sizeof(Sarki));
    }
    izlened_free(havuz, n_sarki * sizeof(Sarki*));
    izlened_free(listeler, n_liste * sizeof(CalmaListesi*));
}

int main() {
    int N_SARKI = 10000;
    int N_LISTE = 5000;
    int SARKI_PER_LISTE = 50;
    struct timespec t1, t2;

    printf("\n=== KOPYA MODELI ===\n");
    izci_sifirla();
    clock_gettime(CLOCK_MONOTONIC, &t1);
    kopya_modeli_test(N_SARKI, N_LISTE, SARKI_PER_LISTE);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    bellek_raporu_yazdir();
    printf("Sure: %.2f ms\n", ms_olc(t1, t2));

    printf("\n=== POINTER MODELI ===\n");
    izci_sifirla();
    clock_gettime(CLOCK_MONOTONIC, &t1);
    pointer_modeli_test(N_SARKI, N_LISTE, SARKI_PER_LISTE);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    bellek_raporu_yazdir();
    printf("Sure: %.2f ms\n", ms_olc(t1, t2));

    return 0;
}
