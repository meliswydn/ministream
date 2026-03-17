#include <stdio.h>
#include "ministream.h"

/* Linked list üzerinde baştan sona lineer arama yapar O(n) */
Sarki* sarki_ara_liste(Sarki* bas, int id) {
    Sarki* curr = bas;
    while (curr != NULL) {
        if (curr->id == id) {
            return curr;
        }
        curr = curr->sonraki;
    }
    return NULL; // Şarkı bulunamadı
}
