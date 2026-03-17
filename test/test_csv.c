#include <stdio.h>
#include <stdlib.h>
#include "../src/ministream.h"
#include "../src/bellek_izci.h"

int main() {
    printf("===== MiniStream CSV Yukleme Testi =====\n");
    printf("sizeof(Sarki) = %zu\n", sizeof(Sarki)); // [cite: 382]

    izci_sifirla();
    int toplam = 0;
    
    // Önemli: Eğer testi ~/ministream içinden çalıştırıyorsan "data/sarkilar.csv" yap.
    // Eğer ~/ministream/test içinden çalıştırıyorsan "../data/sarkilar.csv" kalsın.
    Sarki* bas = csv_yukle("../data/sarkilar.csv", 10000, &toplam); // [cite: 365]

    if (bas == NULL) {
        // Hata devam ederse alternatif yolu dene
        bas = csv_yukle("data/sarkilar.csv", 10000, &toplam);
    }

    if (bas != NULL) {
        bellek_raporu_yazdir(); // [cite: 367]
        // Temizlik döngüsü
        Sarki* curr = bas;
        while (curr) {
            Sarki* sonraki = curr->sonraki;
            izlened_free(curr, sizeof(Sarki));
            curr = sonraki;
        }
    } else {
        printf("HATA: Dosya hiçbir yolda bulunamadi!\n");
    }
    return 0;
}
