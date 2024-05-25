#include <stdio.h>

/*
 T1D6
 BOMBELKOWE SORTOWANIE NA WSKAZNIKACH HEHE
 Powodzenia bombelku, napisz ciało funkcji sortujPrzezWskazniki.
 Aby spełniała swoje zadanie i posortowała liczby rosnąco.
 */

void sortujPrzezWskazniki(int *tab, int rozmiar) {
    for (int j = 0; j < rozmiar -1; j++)
    {
        for (int i = 0; i < rozmiar - 1; i++)
        {
            if(tab[i] >= tab[i+1])
            {
                int temp = tab[i];
                tab[i] = tab[i+1];
                tab[i+1] = temp;
            }
            printf("%d, %d\n",j,i);
        }
    }
}

int main() {
    int liczby[] = {34, 67, 23, 28, 98, 15};
    int rozmiar = sizeof(liczby) / sizeof(liczby[0]);

    printf("Tablica przed sortowaniem: \n");
    for (int i = 0; i < rozmiar; i++) {
        printf("%d ", liczby[i]);
    }
    printf("\n");

    sortujPrzezWskazniki(liczby, rozmiar);

    printf("Tablica po sortowaniu: \n");
    for (int i = 0; i < rozmiar; i++) {
        printf("%d ", liczby[i]);
    }
    printf("\n");

    return 0;
}