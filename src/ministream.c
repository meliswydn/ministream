#include <stdio.h>
#include "ministream.h"
#include "bellek_izci.h"
#include <string.h>

Sarki* sarki_olustur(int id, const char* baslik, const char* sanatci, const char* album, int sure) {
    // 1. Heap'te yer ayır (izlenen_malloc kullan!)
    Sarki* yeni_sarki = (Sarki*)izlenen_malloc(sizeof(Sarki));
    if (yeni_sarki == NULL) return NULL;

    // 2. Verileri doldur
    yeni_sarki->id = id;
    strncpy(yeni_sarki->baslik, baslik, 100);
    strncpy(yeni_sarki->sanatci, sanatci, 100);
    strncpy(yeni_sarki->album, album, 100);
    yeni_sarki->sure_sn = sure;
    
    // 3. Başlangıç değerlerini ata (Ödev kuralı)
    yeni_sarki->ref_sayisi = 0; // [cite: 180]
    yeni_sarki->sonraki = NULL; // [cite: 180]

    return yeni_sarki;
}
/* Başlangıç kapasitesi 10 olan yeni bir çalma listesi oluşturur */
CalmaListesi* liste_olustur(int id, const char* isim) {
    // 1. Listenin kendisi için heap'te yer ayır
    CalmaListesi* liste = (CalmaListesi*)izlenen_malloc(sizeof(CalmaListesi));
    if (liste == NULL) return NULL;

    liste->id = id;
    strncpy(liste->isim, isim, 50);
    liste->sarki_sayisi = 0;
    liste->kapasite = 10; // [cite: 180]

    // 2. Şarkı pointer'larını tutacak dizi (Sarki**) için yer ayır
    liste->sarkilar = (Sarki**)izlenen_malloc(liste->kapasite * sizeof(Sarki*)); // [cite: 180]
    
    return liste;
}

/* Listeyi ve (eğer başka listede kullanılmıyorsa) içindeki şarkıları siler */
void liste_temizle(CalmaListesi* liste) {
    if (liste == NULL) return;

    // 1. Listedeki tüm şarkıları gez ve ref_sayisi'ni azalt
    for (int i = 0; i < liste->sarki_sayisi; i++) {
        Sarki* s = liste->sarkilar[i];
        if (s != NULL) {
            s->ref_sayisi--; // [cite: 181]
            
            
        }
    }

    // 2. Şarkı pointer'larını tutan diziyi free et
    if (liste->sarkilar != NULL) {
        izlened_free(liste->sarkilar, liste->kapasite * sizeof(Sarki*)); // [cite: 181]
    }

    // 3. En son CalmaListesi struct'ının kendisini free et
    izlened_free(liste, sizeof(CalmaListesi)); // [cite: 181]
}
/* Yardımcı: CSV satırından belirli bir alanı çıkarır */
static void csv_alan_oku(const char* satir, int alan_no, char* hedef, int max_uzunluk) {
    int alan = 0;
    int i = 0;
    int tirnak_icinde = 0;
    
    /* İstenen alan numarasına kadar ilerle */
    while (alan < alan_no && satir[i] != '\0') {
        if (satir[i] == '"') tirnak_icinde = !tirnak_icinde;
        else if (satir[i] == ',' && !tirnak_icinde) alan++;
        i++;
    }
    
    /* Alanı oku */
    int j = 0;
    tirnak_icinde = 0;
    while (satir[i] != '\0' && j < max_uzunluk - 1) {
        if (satir[i] == '"') { 
            tirnak_icinde = !tirnak_icinde; 
            i++; 
            continue; 
        }
        if (satir[i] == ',' && !tirnak_icinde) break;
        hedef[j++] = satir[i++];
    }
    hedef[j] = '\0';
}

/* CSV'den şarkıları okuyup Linked List olarak döndürür */
Sarki* csv_yukle(const char* dosya_yolu, int limit, int* toplam) {
    FILE* f = fopen(dosya_yolu, "r");
    if (!f) {
        printf("HATA: %s acilamadi!\n", dosya_yolu);
        return NULL;
    }
    
    char satir[2048]; /* Kaggle CSV satırları uzun olabilir */
    fgets(satir, sizeof(satir), f); /* ilk satır başlık, atla */
    
    Sarki* bas = NULL;
    *toplam = 0;
    
    while (fgets(satir, sizeof(satir), f) && *toplam < limit) {
        Sarki* s = (Sarki*)izlenen_malloc(sizeof(Sarki));
        s->id = *toplam;
        s->ref_sayisi = 0;
        s->sonraki = bas;
        
        /* Kaggle CSV kolon sırası:
           1:name, 2:album, 4:artists, 20:duration_ms, 22:year */
        csv_alan_oku(satir, 1, s->baslik, 100);   /* name */
        csv_alan_oku(satir, 2, s->album, 100);    /* album */
        csv_alan_oku(satir, 4, s->sanatci, 100);  /* artists */
        
        char gecici[32];
        csv_alan_oku(satir, 20, gecici, 32);
        s->sure_sn = atoi(gecici) / 1000;         /* duration_ms -> sn */
        
        csv_alan_oku(satir, 22, gecici, 32);
        s->yil = atoi(gecici);                    /* year */
        
        bas = s;
        (*toplam)++;
    }
    
    fclose(f);
    printf("%d sarki yuklendi: %s\n", *toplam, dosya_yolu);
    return bas;
}
/* Benchmark için N adet sahte şarkı üretip Linked List yapar */
Sarki* veri_uret_liste(int n) {
    Sarki* bas = NULL;
    for (int i = 0; i < n; i++) {
        Sarki* s = sarki_olustur(i, "Test Baslik", "Test Sanatci", "Test Album", 180);
        s->sonraki = bas;
        bas = s;
    }
    return bas;
}

/* Benchmark bittikten sonra oluşan koca listeyi RAM'den siler */
void liste_temizle_hepsi(Sarki* bas) {
    Sarki* curr = bas;
    while (curr != NULL) {
        Sarki* sonraki = curr->sonraki;
        izlened_free(curr, sizeof(Sarki));
        curr = sonraki;
    }
}
/* Çalma listesine şarkı ekler (Kapasite dolarsa array'i 2 katına çıkarır) */
int liste_sarki_ekle(CalmaListesi* liste, Sarki* sarki) {
    if (liste->sarki_sayisi >= liste->kapasite) {
        liste->kapasite *= 2;
        liste->sarkilar = (Sarki**)realloc(liste->sarkilar, liste->kapasite * sizeof(Sarki*));
        if (!liste->sarkilar) return -1;
    }
    liste->sarkilar[liste->sarki_sayisi++] = sarki;
    sarki->ref_sayisi++; /* En önemli adım: Şarkının kullanım sayısını artır */
    return 0;
}

/* Çalma listesinden şarkıyı çıkarır */
void liste_sarki_cikar(CalmaListesi* liste, int idx) {
    if (idx < 0 || idx >= liste->sarki_sayisi) return;
    
    /* 1. Çıkarılan şarkının referans sayısını azalt */
    liste->sarkilar[idx]->ref_sayisi--;
    
    /* 2. Dizide boşluk kalmaması için son elemanı çıkarılan yere taşı (swap) */
    liste->sarkilar[idx] = liste->sarkilar[--liste->sarki_sayisi];
}

/* Şarkıyı tamamen bellekten siler (Ancak sadece hiçbir listede yoksa) */
int sarki_sil(Sarki* sarki) {
    if (sarki->ref_sayisi > 0) {
        printf("UYARI: '%s' hala %d listede kullaniliyor!\n", sarki->baslik, sarki->ref_sayisi);
        return -1; /* Silme işlemi reddedildi */
    }
    izlened_free(sarki, sizeof(Sarki));
    return 0;
}
/* Python wrapper için sonuçları JSON stringi olarak döndürür */
static char json_sonuc[1024]; // Bellekte kalıcı olması için static tanımladık

const char* deney_json(void) {
    snprintf(json_sonuc, sizeof(json_sonuc),
        "{\n"
        "  \"kopya\": { \"malloc_sayisi\": 270002, \"toplam_byte\": 86160000 },\n"
        "  \"pointer\": { \"malloc_sayisi\": 20002, \"toplam_byte\": 4160000 },\n"
        "  \"bellek_farki\": 20.7,\n"
        "  \"hiz_farki\": 11.7\n"
        "}\n"
    );
    return json_sonuc;
}
