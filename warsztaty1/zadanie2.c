#include <stdio.h>

int main(int argc, char** argv){
    int number1;
    char znak[2];
    int number2;
    int wynik = 0;
    printf("Podaj liczbę a: ");
    scanf("%d", &number1);
    printf("Podaj znak * / + -: ");
    scanf("%s", znak);
    printf("Podaj liczbę b: ");
    scanf("%d", &number2);



    if(znak[0] == '*')
    {
        wynik = number1 * number2;
    }
    else if(znak[0] == '/')
    {
        wynik = number1 / number2;
    }
    else if(znak[0] == '+')
    {
        wynik = number1 + number2;
    }
    else if(znak[0] == '-')
    {
        wynik = number1 - number2;
    }
    else
    {
        printf("Niepoprawny znak\n");
    }

    printf("Wynik to: %d\n", wynik);
    return 0;
}