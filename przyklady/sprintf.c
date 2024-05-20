#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    char* buffer = malloc(1024);

    sprintf(buffer, "%s = %d", "dupa", 2137);

    printf("%s\n", buffer);

    free(buffer);

    return 0;
}
