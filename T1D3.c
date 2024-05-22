#include <stdio.h>
#include <stdbool.h>

/*
 Zaimplementuj ciało funkcji isPrime która zwróci true jeśli podany int §num§ jest liczba pierwszą.
 Zwróć false w przeciwnym wypadku.
 */

bool isPrime(int num) {
    if (num <= 1)
    {
        return false;
    }
    if (num <= 3)
    {
        return true;
    }
    if (num % 2 == 0 || num % 3 == 0)
    {
        return false; // jeśli podzielna przez 2,3
    }

    // Sprawdzanie nieparzystych dzielników od 3 do połowy liczby
    for (int i = 3; i <= num / 2; i += 2)
    {
        if (num % i == 0)
        {
            return false;
        }
    }


    return true;
}

void selfTest(void) {
    printf("Self-checking code:\n");
    printf("Test 1: %s\n", isPrime(2) ? "POPRAWNY" : "NIEPRAWIDŁOWY");
    printf("Test 2: %s\n", isPrime(11) ? "POPRAWNY" : "NIEPRAWIDŁOWY");
    printf("Test 3: %s\n", isPrime(99) ?  "NIEPRAWIDŁOWY" : "POPRAWNY");
    printf("Test 4: %s\n", isPrime(97) ? "POPRAWNY" : "NIEPRAWIDŁOWY");
}

int main(void) {
    int num;
    printf("Podaj liczbe: ");
    scanf("%d", &num);

    if (isPrime(num)) {
        printf("%d jest liczba pierwsza.\n", num);
    } else {
        printf("%d nie jest liczba pierwsza.\n", num);
    }

    // Run the self-checking code
    selfTest();

    return 0;
}