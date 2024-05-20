/* NIE DOTYKAJ */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//deklaracja funkcji przyjmuje jako argumenty wskaźnik ss oraz wartość int sz
void weryfikacja1(void *ss, int sz) {

    //XXXXXXXX
    //OOOOOOOO OOOOOOOO OOOOOOOO 24 bajt
    //tablica/string st przyjmuje wskaźnik ss jako typ char
    char *st = (char *) ss;

    //zmienna wiek przyjmuje wartość pierwszego elementu (nie wiem zabardzo dlaczego pierwszą?) z tablicy st
    int wiek = *st;

    //sprawdzamy czy wiek nie równa się 105, jeśli się nie równa wypisujemy błąd i zwracamy funkcję
    if (wiek != 105) {
        printf("Błędne rozwiązanie\n");
        return;
    }

    //         XXXXXXXX
    //OOOOOOOO OOOOOOOO OOOOOOOO 24 bajt
    //wiek ^^  imie^^   wzrost^^
    //st doda do swojego wskaźnika rozmiar struktury podzielony przez 3 (w tym przypadku jest to 8, co oznacza że przechodzimy o 8 bajt do przodu???)
    st += sz / 3;


    //delklarujemy nową zmienną imiePtr typu int dodatni wielkości 64bit (8bajt) która przyjmuje z wartość st jako typ uint64_t
    uint64_t imiePtr = (*(uint64_t *) st);
    //deklarujemy tablice/string imię która przyjmuje wartość imiePtr jako typ char
    char *imie = (char *) imiePtr;

    //                  XXXXXXXX
    //OOOOOOOO OOOOOOOO OOOOOOOO 24 bajt
    //wiek ^^  imie^^   wzrost^^
    //st ponownie doda do swojego wskaźnika rozmiar struktury podzielony przez 3 (w tym przypadku jest to 8, co oznacza że przechodzimy o 8 bajt do przodu???)
    st += sz / 3;

    //za pomocą funkcji strcmp() sprawdzamy różnicę znaków między stringiem imie a "Jan", jeśli różnica poniędzy nimi jest inna od 0 wypisujemy błąd i zwracamy funkcję X
    if (strcmp(imie, "Jan") != 0) {
        printf("Błędne rozwiązanie\n");
        return;
    }

    //deklarujemy zmienną wzrost typu float która przyjmuje wartość st jako typ float
    float wzrost = *(float *) st;
    //sprawdzamy czy wzrost jest między widełkami, jesli true to wygrałeś jeśli nie tru a fols to przgrałeś sadeg
    if (wzrost < 2.138 && wzrost > 2.136) {
        printf("Brawo poprawne rozwiązanie\n");
    } else {
        printf("Ups, błędne rozwiązanie\n");
    }
}

/* KONIEC SEKCJI NIE DOTYKAJ */

/*
 Treść zadania:

 Stwórz strukturę o nazwie `czlowiek`.
 Zrób żeby zawierała następujące pola:
 - `wiek` <- liczba całkowita (rozmiar standardowy)
 - `imie` <- no jego imie a co xD (ptr do stringa)
 - `wzrost` <- liczba zmiennoprzecinkowa w metrach amerykańskich np 1.234

 Stwórz człowieka o imieniu `Jan`, wieku 105 lat, wzroście 2 m 137 cm (w metrach amerykańskich - 1 metr = 1000 cm - 2m 137cm = 2.137)
 Pola wiek, imie, i wzrost muszą mieć taką samą koleność.

 Uruchom funckje `weryfikacja1` ze swojej funkcji main podając wskaźnik do człowieka oraz rozmiar struktury człowiek jako drugi parametr.

 ***** Zadanie na 6 albo gwiazdke jak zwał tak zwał *****
 Opisz co robi po kolei funckja `void weryfikacja1(void* ss, int sz)`
*/

int main(void) {
    struct czlowiek {
        int wiek;
        char *imie;
        float wzrost;
    };

    struct czlowiek jan;
    jan.wiek = 105;
    jan.imie = "Jan";
    jan.wzrost = 2.137;

    int a = sizeof(jan.wiek);
    int b = sizeof(jan.imie);
    int c = sizeof(jan.wzrost);
    int d = sizeof(jan);
    printf("%d + %d + %d = %d ahaczemu?\n", a, b, c, d);


    weryfikacja1(&jan, sizeof(jan));
}
