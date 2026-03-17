#include <stdio.h>
#include "hash_map.h"
#include "bellek_izci.h"

/* Şarkı ID'sini 1024 kovalık tabloya sığdırmak için mod alma işlemi */
int hash_fonk(int id) {
    return id % TABLO_BOYUTU;
}

/* Boş hash map oluştur, tüm kovaları NULL yap */
HashMap* hashmap_olustur(void) {
    HashMap* map = (HashMap*)izlenen_malloc(sizeof(HashMap));
    if (map == NULL) return NULL;
    
    for (int i = 0; i < TABLO_BOYUTU; i++) {
        map->kovalar[i] = NULL;
    }
    return map;
}

/* Hash map'e şarkı ekle (Chaining/Zincirleme yöntemi ile) */
void hashmap_ekle(HashMap* map, Sarki* sarki) {
    int idx = hash_fonk(sarki->id);
    
    /* Aynı kovaya denk gelen şarkıları linked list olarak birbirine bağlar */
    sarki->sonraki = map->kovalar[idx]; 
    map->kovalar[idx] = sarki;
}

/* Hash map'te şarkı ara O(1) ortalama */
Sarki* sarki_ara_map(HashMap* map, int id) {
    int idx = hash_fonk(id);
    /* Doğru kovaya git ve o kovanın içindeki kısa zincirde şarkıyı ara */
    return sarki_ara_liste(map->kovalar[idx], id);
}

/* Hash map'i temizle: hem haritayı hem de içindeki şarkıları free et */
void hashmap_temizle(HashMap* map) {
    if (map == NULL) return;
    
    for (int i = 0; i < TABLO_BOYUTU; i++) {
        Sarki* curr = map->kovalar[i];
        while (curr != NULL) {
            Sarki* sonraki = curr->sonraki;
            izlened_free(curr, sizeof(Sarki));
            curr = sonraki;
        }
    }
    izlened_free(map, sizeof(HashMap));
}
/* Benchmark için N adet sahte şarkı üretip Hash Map'e doldurur */
HashMap* veri_uret_map(int n) {
    HashMap* map = hashmap_olustur();
    for (int i = 0; i < n; i++) {
        Sarki* s = sarki_olustur(i, "Test Baslik", "Test Sanatci", "Test Album", 180);
        hashmap_ekle(map, s);
    }
    return map;
}
