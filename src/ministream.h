/* ministream.h */
#ifndef MINISTREAM_H
#define MINISTREAM_H
#include <stdlib.h>

/* Şarkı */
typedef struct Sarki {
    int id;
    char baslik[100];
    char sanatci[100];
    char album[100];
    int sure_sn;
    int yil;
    int ref_sayisi;
    struct Sarki* sonraki;
} Sarki;

/* Çalma Listesi */
typedef struct {
    int id;
    char isim[50];
    Sarki** sarkilar; /* heap'teki pointer dizisi */
    int sarki_sayisi;
    int kapasite;
} CalmaListesi;

/* Kullanıcı */
typedef struct {
    int id;
    char isim[50];
    CalmaListesi** listeler;
    int liste_sayisi;
} Kullanici;

/* Fonksiyon İmzaları */
Sarki* sarki_olustur(int id, const char* baslik, const char* sanatci, const char* album, int sure);
Sarki* csv_yukle(const char* dosya_yolu, int limit, int* toplam);
#endif
Sarki* veri_uret_liste(int n);
CalmaListesi* liste_olustur(int id, const char* isim);
void liste_temizle(CalmaListesi* liste);
void liste_temizle_hepsi(Sarki* bas);
int liste_sarki_ekle(CalmaListesi* liste, Sarki* sarki);
void liste_sarki_cikar(CalmaListesi* liste, int idx);
int sarki_sil(Sarki* sarki);
