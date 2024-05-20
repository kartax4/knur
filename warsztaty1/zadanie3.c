#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    int n = 0;
    int first = 0;
    int second = 1;
    int next = 0;

    printf("Podaj liczbę znaków ciągu fibonacciego\n");
    scanf("%d", &n);
    strtol();

        for (int i = 0; i < n; i++) {
            if(i <= 1)
            {
                next = i;
            }
            else
            {
                next += first;
                first = second;
                second = next;
            }
            printf("%d. %d\n", i, next);
        }
    return 0;
}