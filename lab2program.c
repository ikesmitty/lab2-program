#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main()
{
    char data1[16];

    printf ("Please input username: ");
    fgets(data1, SIZE, stdin);
    printf ("you entered: %s\n", data1);
    return 0;
}

