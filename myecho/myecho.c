#include <stdio.h>

int main(int argc, char *argv[])
{
    // printf("arguments number is : %d\n", argc - 1);
    int i;
    for (i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}
