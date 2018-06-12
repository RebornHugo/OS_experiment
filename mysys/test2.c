#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int split(char str[])
{
    printf("%s\n", str);
    // char str[] = "echo qwe sdfsa sdaf";
    char **res = NULL;
    char *p = strtok(str, " ");
    int n_spaces = 0, i;

    /* split string and append tokens to 'res' */

    while (p)
    {
        res = realloc(res, sizeof(char *) * ++n_spaces);

        if (res == NULL)
            exit(-1); /* memory allocation failed */

        res[n_spaces - 1] = p;

        p = strtok(NULL, " ");
    }

    /* realloc one extra element for the last NULL */

    res = realloc(res, sizeof(char *) * (n_spaces + 1));
    res[n_spaces] = 0;

    for (i = 0; i < (n_spaces + 1); ++i)
        printf("res[%d] = %s\n", i, res[i]);

    /* free the memory allocated */

    free(res);
    return 1;
}

int main(int argc, char *argv[])
{
    printf("--------------------------------------------------\n");
    char str[] = "echo HELLO WORLD nmm";
    split(str);
    return 0;
}