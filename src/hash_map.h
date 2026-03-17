#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "ministream.h"

#define TABLO_BOYUTU 1024

typedef struct {
    Sarki* kovalar[TABLO_BOYUTU]; /* Şarkı pointer'larını tutan 1024 kova */
} HashMap;

/* Fonksiyon İmzaları */
HashMap* hashmap_olustur(void);
void hashmap_ekle(HashMap* map, Sarki* sarki);
Sarki* sarki_ara_map(HashMap* map, int id);
void hashmap_temizle(HashMap* map);

/* Linked list arama fonksiyonunu da burada tanıtalım ki diğer dosyalar görsün */
Sarki* sarki_ara_liste(Sarki* bas, int id);

#endif
HashMap* veri_uret_map(int n);
