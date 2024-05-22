#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    char str1[100];
    char str2[100];
    printf("Podaj string\n");
    fgets(str1, 10, stdin);
    fgets(str2, 100, stdin);

    printf("%s", str1);
    printf("%s", str2);
    return 0;
}