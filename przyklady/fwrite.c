#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    const char* uuh = "daj suba widzu bo jestem menelem\npotrzebuje na piwo";
    const char* plikDoZapisu = "piwo.txt";
    FILE* uchwyt = fopen(plikDoZapisu, "w");
    if(uchwyt == NULL)
    {
        printf("Nie udało się otworzyć pliku do zapisu\n");
        return -1;
    }

    size_t ret = fwrite(uuh, 1, strlen(uuh), uchwyt);
    if(ret == strlen(uuh))
    {
        printf("Operacja się powiodła\n");
    }

    fclose(uchwyt);
    return 0;
}
