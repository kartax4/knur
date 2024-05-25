#include <stdio.h>
#include <stdlib.h>

/*
 T1D5 - Pamiętaj bo pamięć to rzecz święta xD (pzdr bonusrpk)
 Zarządzanie pamięcią - utrwalenie
 22.05.2025
 
 
 Postępuj zgodnie z instrukcjami w komentarzach, i dopisz brakujący kod.
 Zwróć uwagę na użycie funkcji calloc i free.
 Zastanów się, dlaczego używamy calloc zamiast malloc.
 */

int main(void) {
    // Zadanie 1: Poproś użytkownika o podanie liczby elementów
    int n;
    int *arr;

    // Zadanie 1: Poproś użytkownika o podanie liczby elementów
    printf("Podaj liczbe elementow: ");
    scanf("%d", &n);

    // Zadanie 2: Przydziel pamięć dynamicznie dla 'n' elementów używając calloc
    arr = calloc(n, sizeof(int));
    // Zadanie 3: Sprawdź, czy pamięć została pomyślnie przydzielona
    if (arr == NULL) {
        printf("Blad bo nie wyzerowało\n");
        return 1;
    }
    // Zadanie 4: Wprowadź elementy do przydzielonej pamięci
    printf("Wpisać:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    // Zadanie 5: Wyświetl te elementy
    printf("Wwypisac:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    // Zadanie 6: Zwolnij przydzieloną pamięć
    free(arr);
    return 0;
}